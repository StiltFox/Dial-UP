/*******************************************************
* Created by Cryos on 6/5/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <cstring>
#include <queue>
#include <chrono>
#include <thread>
#include "Socket.h++"

using namespace std;

namespace StiltFox::DialUp::Http
{
    inline bool setOptions(int handle)
    {
    #ifdef MAC
        return true;
    #else
        int opt = 1;
        return setsockopt(handle, SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt)) >= 0;
    #endif
    }

    inline queue<char> getRawData(int socketID, long maxBytes)
    {
        queue<char> output;
        int bufferSize = 1024 > maxBytes ? maxBytes : 1024;
        long numPasses = maxBytes / bufferSize;
        char buffer[bufferSize];

        if (maxBytes % bufferSize > 0) numPasses++;

        for (int x = 0; x < numPasses; x++)
        {
            memset(buffer, 0, bufferSize);
            read(socketID, buffer, bufferSize);

            for (int y = 0; y < bufferSize; y++)
            {
                if(buffer[y] == '\000') break;
                output.push(buffer[y]);
            }

            if (buffer[bufferSize - 1] == 0) break;
        }

        return output;
    }

    void Connection::initializeValues(int portNumber, int queueSize)
    {
        socketHandle = -1;
        connectionHandle = -1;
        queue = queueSize;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(portNumber);
    }

    bool Connection::openSocket()
    {
        bool output = false;

        if ((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) >= 0)
        {
            if (setOptions(socketHandle))
            {
                if (bind(socketHandle,reinterpret_cast<sockaddr*>(&address),sizeof(address)) >= 0)
                {
                    if(listen(socketHandle, queue) >= 0) output = true;
                }
            }
        }

        return output;
    }

    shared_ptr<Connection> Connection::openConnection(int portNumber, int queueSize)
    {
        auto output = make_shared<Connection>();

        output->initializeValues(portNumber, queueSize);
        if (output->openSocket())
        {
            int addressLength = sizeof(output->address);
            output->connectionHandle =
                accept(output->socketHandle, (sockaddr *)&output->address, (socklen_t *)& addressLength);
        }
        else
        {
            output = nullptr;
        }

        return output;
    }

    HttpMessage Connection::receiveData(long maxBytes, long maxWaitTime) const
    {
        HttpMessage output = HttpMessage::ERROR;
        std::queue<char> rawData;

        thread dataThread([connectionHandle, maxBytes, &rawData]()
        {
            rawData = getRawData(connectionHandle, maxBytes);
        });

        return output;
    }

    Connection::~Connection()
    {
        if (connectionHandle > -1)
        {
            close(connectionHandle);
            connectionHandle = -1;
        }

        if (socketHandle > -1)
        {
            shutdown(socketHandle, SHUT_RDWR);
            socketHandle = -1;
        }
    }
}
