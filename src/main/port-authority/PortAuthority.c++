/*******************************************************
* Created by Cryos on 6/20/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <thread>
#include "PortAuthority.h++"

using namespace std;

namespace StiltFox::DialUp
{
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

    const HttpMessage PortAuthority::KILL_MESSAGE = {HttpMessage::Method::DELETE,"*",{"operation","kill"},""};

    PortAuthority::PortAuthority(int portNumber, int killPortNumber, int maxWorkerThreads, long maxWaitTime,
        long maxDataSize)
    {
        socket = make_shared<Socket>(portNumber);
        killSocket = make_shared<Socket>(killPortNumber);
        maxThreads = maxWorkerThreads;
        this->maxWaitTime = maxWaitTime;
        this->maxDataSize = maxDataSize;
        workers = new thread*[maxWorkerThreads];

        thread killThread(listenForKillCommand); //We will not track this later. Just let it go.
        for (int x=0; x<maxWorkerThreads; x++) workers[x] = new thread(connectionThreadHandler);
    }

    void PortAuthority::connectionThreadHandler()
    {
        while (socket->isOpen())
        {
            auto connection = Socket::Connection::openConnection(*socket, maxWaitTime, maxDataSize);
            auto request = connection->listen();

            if (request.errorMessage.empty())
            {
                HttpMessage message = request.data;
                HttpMessage response = registry.submitMessage(message);
                connection->sendData(response.printAsResponse());
            }
            else
            {
                connection->sendData(generateErrorFromResponse(request.errorMessage).printAsResponse());
            }
        }
    }

    void PortAuthority::listenForKillCommand()
    {
        const auto connection = Socket::Connection::openConnection(*killSocket, 500, 1000);
        bool cont = true;

        while (cont)
        {
            auto request = connection->listen();

            if (request.errorMessage.empty())
            {
                HttpMessage requestMessage = request.data;

                if (requestMessage == KILL_MESSAGE)
                {
                    HttpMessage response = {200,{{"Content-Type",{"text/plain"}}},"shutting down"};
                    cont = false;
                    socket->closePort();
                    connection->sendData(response.printAsResponse());
                }
                else
                {
                    HttpMessage failResponse = {400,{{"Content-Type",{"text/plain"}}},"unknown command"};
                    connection->sendData(failResponse.printAsResponse());
                }
            }
            else
            {
                connection->sendData(generateErrorFromResponse(request.errorMessage).printAsResponse());
            }
        }
    }
}