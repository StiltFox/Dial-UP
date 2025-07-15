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
#include "ClientConnection.h++"

using namespace std;

namespace StiltFox::DialUp
{
    mutex readLock;

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
        const long bufferSize = maxDataSizeBytes > 1024 ? 1024 : maxDataSizeBytes;
        long numIterations = bufferSize / maxDataSizeBytes;
        char buffer[bufferSize];

        if (bufferSize % maxDataSizeBytes > 0) numIterations++;

        do
        {
            memset(buffer, 0, bufferSize);

            numIterations--;
            if (numIterations < 0)
            {
                lock_guard guard(readLock);
                rawData.errorMessage = "Data received exceeds limit";
                break;
            }

            read(handle, buffer, bufferSize);
            for (int y=0; y < bufferSize; y++)
            {
                lock_guard guard(readLock);
                rawData.data.emplace_back(buffer[y]);
            }
        } while ((buffer[bufferSize -1] != 0) && !holt);

        lock_guard guard(readLock);
        holt = true;
    }


    Response ClientConnection::receiveData()
    {
        Response output = {{},""};
        bool holt = false;

        int addressLength = sizeof(sockaddr_in);
        handle = accept(socketHandle, (sockaddr*)&address,(socklen_t*)&addressLength);

        auto startTime = chrono::high_resolution_clock::now();
        thread socketThread([&output, this, &holt]()
        {
            readSocketToBuffer(output, holt);
        });

        while (chrono::high_resolution_clock::now() - startTime < chrono::milliseconds(maxWaitTimeMs) && !holt);

        if (!holt)
        {
            lock_guard guard(readLock);
            holt = true;
            output.errorMessage = "Connection timed out";
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