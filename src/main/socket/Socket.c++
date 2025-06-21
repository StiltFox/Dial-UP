/*******************************************************
* Created by Cryos on 6/5/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifdef MAC
    #include <sys/types.h>
#endif
#include <cstring>
#include <queue>
#include <mutex>
#include <chrono>
#include <thread>
#include "Socket.h++"

using namespace std;

namespace StiltFox::DialUp
{
    mutex readLock;

    inline bool setOptions(int handle)
    {
    #ifdef MAC
        return true;
    #else
        int opt = 1;
        return setsockopt(handle, SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt)) >= 0;
    #endif
    }

    Socket::Socket(const int portNumber)
    {
        handle = -1;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(portNumber);
    }

    bool Socket::openPort(const int queueSize)
    {
        bool output = false;

        if ((handle = socket(AF_INET, SOCK_STREAM, 0)) >= 0)
        {
            if (setOptions(handle))
            {
                if (bind(handle,(struct sockaddr*)&address,sizeof(address)) >= 0)
                {
                    if(listen(handle, queueSize) >= 0) output = true;
                }
            }
        }

        if (!output) closePort();

        return output;
    }

    bool Socket::isOpen() const
    {
        return handle > -1;
    }

    void Socket::closePort()
    {
        if (handle > -1)
        {
            shutdown(handle,SHUT_RDWR);
            handle = -1;
        }
    }

    Socket::~Socket()
    {
        closePort();
    }

    Socket::Connection::Connection(const int handle, const long maxWaitTimeMS, const long maxDataSizeBytes)
    {
        this->handle = handle;
        this->maxWaitTimeMS = maxWaitTimeMS;
        this->maxDataSizeBytes = maxDataSizeBytes;
    }

    shared_ptr<Socket::Connection> Socket::Connection::openConnection(
            const Socket& socket, long maxWaitTimeMS, long maxDataSizeBytes)
    {
        int addressLength = sizeof(socket.address);
        int connectionHandle = accept(socket.handle, (struct sockaddr*)&socket.address, (socklen_t*)&addressLength);

        return connectionHandle > -1 ?
            make_shared<Connection>(connectionHandle, maxWaitTimeMS, maxDataSizeBytes) : nullptr;
    }

    void Socket::Connection::readSocketToBuffer(Response& rawData, bool& holt)
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


    Socket::Connection::Response Socket::Connection::listen()
    {
        Response output = {{}, ""};
        bool holt = false;

        auto startTime = chrono::high_resolution_clock::now();
        thread socketThread([&output, this, &holt]()
        {
            readSocketToBuffer(output, holt);
        });

        while (chrono::high_resolution_clock::now() - startTime < chrono::milliseconds(this->maxWaitTimeMS) && !holt);

        if (!holt)
        {
            lock_guard guard(readLock);
            holt = true;
            output.errorMessage = "Connection timed out";
        }

        return output;
    }

    void Socket::Connection::sendData(const std::vector<char>& data)
    {
        send(handle, data.data(), data.size(), 0);
    }

    Socket::Connection::~Connection()
    {
        if (handle > -1)
        {
            close(handle);
            handle = -1;
        }
    }

}