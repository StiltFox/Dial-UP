/*******************************************************
* Created by Cryos on 7/12/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <mutex>
#include <queue>
#include "ClientConnection.h++"

#include <stack>
#include <nlohmann/detail/input/parser.hpp>

using namespace std;

namespace StiltFox::DialUp
{
    mutex readLock;

    inline void trimWhitespaceFromStartOfMessage(vector<char>& rawData)
    {
        queue dataQueue(deque(rawData.begin(), rawData.end()));
        while (!dataQueue.empty() && (isspace(dataQueue.front()) || iscntrl(dataQueue.front()))) dataQueue.pop();
        rawData.clear();
        while (!dataQueue.empty())
        {
            rawData.emplace_back(dataQueue.front());
            dataQueue.pop();
        }
    }

    ClientConnection::ClientConnection(int socketHandle, sockaddr_in address, long maxWaitTimeMs, long maxDataSizeBytes)
    {
        this->socketHandle = socketHandle;
        this->maxWaitTimeMs = maxWaitTimeMs;
        this->maxDataSizeBytes = maxDataSizeBytes;
        this->address = address;
        handle = -1;
    }

    void ClientConnection::readSocketToBuffer(Response& rawData, bool& holt)
    {
        long numIterations = maxDataSizeBytes / 1024;
        char buffer[1024] = {};

        if (maxDataSizeBytes % 1024 > 0) numIterations++;

        do
        {
            memset(buffer, 0, 1024);

            numIterations--;
            if (numIterations < 0)
            {
                lock_guard guard(readLock);
                rawData.errorMessage = "Data received exceeds limit";
                break;
            }

            read(handle, buffer, 1024);
            for (int y=0; y < 1024; y++)
            {
                lock_guard guard(readLock);
                rawData.data.emplace_back(buffer[y]);
            }
        } while ((buffer[1024 -1] != '\000') && !holt);

        trimWhitespaceFromStartOfMessage(rawData.data);

        lock_guard guard(readLock);
        holt = true;
    }


    Response ClientConnection::receiveData()
    {
        Response output = {{},""};
        bool holt = false;

        socklen_t addressLength = sizeof(address);
        handle = accept(socketHandle, (sockaddr*)&address,&addressLength);

        if (handle == -1)
        {
            output.errorMessage = strerror(errno);
        }
        else
        {
            auto startTime = chrono::high_resolution_clock::now();
            thread socketThread([this, &output, &holt]()
            {
                readSocketToBuffer(output, holt);
            });

            while (chrono::high_resolution_clock::now() - startTime < chrono::milliseconds(maxWaitTimeMs) && !holt);
            socketThread.join();

            if (!holt)
            {
                lock_guard guard(readLock);
                holt = true;
                output.errorMessage = "Connection timed out";
            }
        }

        return output;
    }

    void ClientConnection::sendData(std::vector<char> data)
    {
        if (handle != -1)
        {
            send(handle,data.data(),data.size(),0);
        }
    }

    void ClientConnection::terminate()
    {
        close(handle);
        handle = -1;
    }

    int ClientConnection::getHandle()
    {
        return handle;
    }

    ClientConnection::~ClientConnection()
    {
        terminate();
    }
}