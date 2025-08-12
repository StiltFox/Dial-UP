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

    const HttpMessage PortAuthority::KILL_MESSAGE = {HttpMessage::Method::DELETE,"*",{"operation","kill"},""};

    inline bool PortAuthority::openPort(ServerSocket& toOpen)
    {
        bool output = false;

        if (toOpen.isOpen())
        {
            output = true;
        }
        else
        {
            logger(LogSevarity::INFO, "Opening new socket on port " + toOpen.getAddress().sin_port);
            toOpen.openPort();
            if (toOpen.isOpen())
            {
                logger(LogSevarity::INFO, "Port opened successfully");
            }
            else
            {
                logger(LogSevarity::ERROR, "Failed to open socket on port " + toOpen.getAddress().sin_port);
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

        switch (errorMessage)
        {
        case "Connection timed out":
            errorStatus = 408;
            break;
        case "Data received exceeds limit":
            errorStatus = 413;
            break;
        default:
            errorStatus = 400;
            break;
        }

        return {errorStatus,{},""};
    }

    void PortAuthority::listenForKillCommand()
    {
        ClientConnection connection(*killSocket, 500, 1000);
        bool cont = true;

        while (cont)
        {
            const auto [data, errorMessage] = connection.receiveData();

            if (errorMessage.empty())
            {
                if (HttpMessage requestMessage = data; requestMessage == KILL_MESSAGE)
                {
                    cont = false;
                    socket->closePort();
                    sendResponse({200,{{"Content-Type",{"text/plain"}}},"shutting down"},connection);
                }
                else
                {
                    sendResponse({400,{{"Content-Type",{"text/plain"}}},"unknown command"},connection);
                }
            }
            else
            {
                sendResponse(generateErrorFromResponse(errorMessage),connection);
            }
        }
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

            cout << "[" << LogSeverityToString.at(sevarity) << ": " << put_time(&localTime,"%d-%b-%Y %H:%M:%S") << "]";
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

        if (openPort(*socket))
        {
            if (openPort(*killSocket))
            {
                if (workers == nullptr) workers = new thread*[maxThreads];

                thread killThread(listenForKillCommand); //We will not track this later. Just let it go.
                for (int x=0; x<maxThreads; x++) workers[x] = new thread(connectionThreadHandler);
                logger(LogSevarity::INFO, "Server Started Successfully");
            }
        }
    }

    void PortAuthority::stopApplication()
    {
        socket->closePort();
        killSocket->closePort();

        for (int x=0; x<maxThreads; x++)
        {
            workers[x]->join();
            delete workers[x];
            workers[x] = nullptr;
        }

        delete[] workers;
        workers = nullptr;
    }

    PortAuthority::~PortAuthority()
    {
        stopApplication();
    }
}
//     void PortAuthority::connectionThreadHandler()
//     {
//         while (socket->isOpen())
//         {
//             auto connection = ServerSocket::Connection::openConnection(*socket, maxWaitTime, maxDataSize);
//             auto request = connection->listen();
//
//             if (request.errorMessage.empty())
//             {
//                 HttpMessage message = request.data;
//                 HttpMessage response = registry.submitMessage(message);
//                 connection->sendData(response.printAsResponse());
//             }
//             else
//             {
//                 connection->sendData(generateErrorFromResponse(request.errorMessage).printAsResponse());
//             }
//         }
//     }