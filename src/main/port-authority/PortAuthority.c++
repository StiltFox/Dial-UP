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
        (HttpMessage::Method::DELETE,"*",{{"operation",{"kill"}}},"");

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

    void connectionThreadHandler(const ServerSocket& socket, const long& maxWaitTime, const long& maxDataSize,
        const EndpointRegistry& registry)
    {
        while (socket.isOpen())
        {
            ClientConnection connection(socket,maxWaitTime,maxDataSize);
            const auto [data, errorMessage] = connection.receiveData();

            if (errorMessage.empty())
            {
                auto response = registry.submitMessage(data).printAsResponse();
                connection.sendData({response.begin(), response.end()});
            }
            else
            {
                sendResponse(generateErrorFromResponse(errorMessage),connection);
            }
        }
    }

    void listenForKillCommand(ServerSocket& killSocket)
    {
        // ClientConnection connection(killSocket, 500, 1000);
        // bool cont = true;
        //
        // while (cont)
        // {
        //     const auto [data, errorMessage] = connection.receiveData();
        //     if (errorMessage.empty())
        //     {
        //         if (HttpMessage requestMessage = data; requestMessage == PortAuthority::KILL_MESSAGE)
        //         {
        //             cont = false;
        //             sendResponse({200,{{"Content-Type",{"text/plain"}}},"shutting down"}, connection);
        //         }
        //         else
        //         {
        //             sendResponse({400,{{"Content-Type",{"text/plain"}}},"unknown command"},connection);
        //         }
        //     }
        //     else
        //     {
        //         sendResponse(generateErrorFromResponse(errorMessage), connection);
        //     }
        // }
    }

    PortAuthority::PortAuthority(int portNumber, int killPortNumber, int maxWorkerThreads, long maxWaitTime,
        long maxDataSize)
    {
        socket = make_shared<ServerSocket>(portNumber);
        killSocket = make_shared<ServerSocket>(killPortNumber);
        maxThreads = maxWorkerThreads;
        this->maxWaitTime = maxWaitTime;
        this->maxDataSize = maxDataSize;
        workers = nullptr;
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

        if (maxThreads > 0 && openPort(*socket, logger))
        {
            if (openPort(*killSocket, logger))
            {
                // if (workers == nullptr) workers = new thread*[maxThreads];

                // thread killThread([this]()
                // {
                //     // listenForKillCommand(*killSocket);
                //     // this->socket->closePort();
                // });
                // killThread.detach();//We will not track this later. Just let it go.

                // for (int x=0; x<maxThreads; x++) workers[x] = new thread([this]()
                // {
                //     connectionThreadHandler(*this->socket,this->maxWaitTime,this->maxDataSize,this->registry);
                // });
                logger(LogSevarity::INFO, "Server started successfully");
                // if(workers[0] != nullptr) workers[0]->join();
            }
        }
    }

    void PortAuthority::stopApplication()
    {
        logger(LogSevarity::INFO, "Stopping server");
        socket->closePort();
        killSocket->closePort();
        logger(LogSevarity::INFO, "Ports closed");

        if (workers != nullptr)
        {
            for (int x=0; x<maxThreads; x++)
            {
                if (workers[x] != nullptr)
                {
                    workers[x]->join();
                    delete workers[x];
                    workers[x] = nullptr;
                }
            }
        }
        logger(LogSevarity::INFO, "Workers stopped");

        delete[] workers;
        workers = nullptr;
        logger(LogSevarity::INFO, "Server stopped successfully");
    }

    PortAuthority::~PortAuthority()
    {
        stopApplication();
    }
}