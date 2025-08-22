/*******************************************************
* Created by Cryos on 6/20/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <thread>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <Stilt_Fox/StandMixer/DataProcessor.h++>
#include "PortAuthority.h++"
#include "ClientConnection.h++"

using namespace std;

namespace StiltFox::DialUp
{
    const unordered_map<PortAuthority::LogSevarity, string> LogSeverityToString =
    {
        {PortAuthority::LogSevarity::DEBUG,"DEBUG"},
        {PortAuthority::LogSevarity::INFO,"INFO"},
        {PortAuthority::LogSevarity::WARNING,"WARNING"},
        {PortAuthority::LogSevarity::ERROR,"ERROR"}
    };

    const HttpMessage PortAuthority::KILL_MESSAGE
        (HttpMessage::Method::DELETE,"",{{"operation",{"kill"}}},"");

    inline bool openPort(ServerSocket& toOpen, const std::function<void(PortAuthority::LogSevarity, string)>& logger)
    {
        bool output = false;

        if (toOpen.isOpen())
        {
            output = true;
        }
        else
        {
            const int portNumber = htons(toOpen.getAddress().sin_port);
            logger(PortAuthority::LogSevarity::INFO, "Opening new socket on port " + to_string(portNumber));
            toOpen.openPort();
            if (toOpen.isOpen())
            {
                logger(PortAuthority::LogSevarity::INFO, "Port " + to_string(portNumber) + " opened successfully");
                output = true;
            }
            else
            {
                logger(PortAuthority::LogSevarity::ERROR, "Failed to open socket on port " + to_string(portNumber));
            }
        }

        return output;
    }

    inline void sendResponse(const HttpMessage& message, ClientConnection& listener)
    {
        string response = message.printAsResponse();
        listener.sendData(vector(response.begin(), response.end()));
    }

    inline HttpMessage generateErrorFromResponse(const string& errorMessage)
    {
        int errorStatus = 0;

        if (errorMessage == "Connection timed out")
        {
            errorStatus = 408;
        }
        else if (errorMessage == "Data received exceeds limit")
        {
            errorStatus = 413;
        }
        else
        {
            errorStatus = 400;
        }

        return {errorStatus,{},""};
    }

    bool checkForKillCommand(const Response& data, ClientConnection& connection)
    {
        bool output = false;

        if (data.errorMessage.empty())
        {
            const HttpMessage requestMessage = data.data;
            if (requestMessage.httpMethod == PortAuthority::KILL_MESSAGE.httpMethod &&
                requestMessage.body == PortAuthority::KILL_MESSAGE.body && requestMessage.headers.contains("operation")
                && requestMessage.headers.at("operation") == PortAuthority::KILL_MESSAGE.headers.at("operation"))
            {
                sendResponse({200,{{"Content-Type",{"text/plain"}}},"shutting down"}, connection);
                output = true;
            }
            else
            {
                sendResponse({400,{{"Content-Type",{"text/plain"}}},"unknown command"},connection);
            }
        }
        else
        {
            sendResponse(generateErrorFromResponse(data.errorMessage), connection);
        }

        return output;
    }

    inline void startKillThread(shared_ptr<ServerSocket> killSocket, PortAuthority* authority)
    {
        thread killThread([killSocket, authority]()
        {
            while (killSocket->isOpen())
            {
                ClientConnection killConnection(*killSocket);
                const auto response = killConnection.receiveData();
                if (checkForKillCommand(response, killConnection)) authority->stopApplication();
            }
        });
        killThread.detach();
    }

    void PortAuthority::startWorkerThread(shared_ptr<ClientConnection> connection, Response message)
    {
        thread workerThread([this, connection, message]()
        {
            if (message.errorMessage.empty())
            {
                sendResponse(registry.submitMessage(message.data), *connection);
            }
            else
            {
                sendResponse(generateErrorFromResponse(message.errorMessage),*connection);
            }
            lock_guard lock{threadCountMutex};
            currentThreads--;
        });
        workerThread.detach();
    }

    void PortAuthority::startMainLoop()
    {
        while (socket->isOpen())
        {
            if (currentThreads < maxThreads)
            {
                auto connection = make_shared<ClientConnection>(*socket, maxWaitTime, maxDataSize);
                auto data = connection->receiveData();
                logger(LogSevarity::DEBUG, "Thread starting for: " + string(data.data.begin(), data.data.end()));
                if (socket->isOpen())
                {
                    lock_guard guard(threadCountMutex);
                    startWorkerThread(connection, data);
                    currentThreads++;
                }
            }
        }
    }

    PortAuthority::PortAuthority(int portNumber, int killPortNumber, int maxWorkerThreads, long maxWaitTime,
        long maxDataSize)
    {
        socket = make_shared<ServerSocket>(portNumber);
        killSocket = make_shared<ServerSocket>(killPortNumber);
        maxThreads = maxWorkerThreads;
        currentThreads = 0;
        this->maxWaitTime = maxWaitTime;
        this->maxDataSize = maxDataSize;
        logger = [](LogSevarity sevarity, string message)
        {
            const auto currentTime = time(nullptr);
            const auto localTime = *localtime(&currentTime);

            cout << "[" << LogSeverityToString.at(sevarity) << ": " << put_time(&localTime,"%d-%b-%Y %H:%M:%S") << "] ";
            const auto tokens = StandMixer::DataProcessor::tokenize(message, "\r\n");

            if (tokens.size() > 1)
            {
                cout << endl;
                for (const auto token : tokens) cout <<  "    " << token << endl;
            }
            else
            {
                cout << message << endl;;
            }
        };
    }

    void PortAuthority::startApplication(const std::string& banner)
    {
        if (!banner.empty()) cout << banner << endl << endl;
        logger(LogSevarity::INFO, "Starting Application");

        if (maxThreads > 0 && openPort(*socket, logger) && openPort(*killSocket, logger))
        {
            startKillThread(killSocket,this);
            logger(LogSevarity::INFO, "Server started successfully");
            startMainLoop();
        }
    }

    void PortAuthority::stopApplication()
    {
        logger(LogSevarity::INFO, "Stopping server");
        socket->closePort();
        killSocket->closePort();
        logger(LogSevarity::INFO, "Ports closed");
        logger(LogSevarity::INFO, "Server stopped successfully");
    }

    PortAuthority::~PortAuthority()
    {
        if (socket->isOpen() || killSocket->isOpen()) stopApplication();
        while (currentThreads>0); //wait for threads to finish
    }
}