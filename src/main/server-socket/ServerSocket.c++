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
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include "ServerSocket.h++"

#include <iostream>

using namespace std;

namespace StiltFox::DialUp
{
    inline bool setOptions(int handle)
    {
    #ifdef MAC
        return true;
    #else
        int opt = 1;
        return setsockopt(handle, SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) >= 0;
    #endif
    }

    ServerSocket::ServerSocket(int portNumber)
    {
        handle = -1;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(portNumber);
    }

    bool ServerSocket::openPort(int queueSize)
    {
        bool output = false;

        if ((handle = socket(AF_INET, SOCK_STREAM, 0)) >= 0)
        {
            if (setOptions(handle))
            {
                if (bind(handle,(sockaddr*)&address,sizeof(address)) >= 0)
                {
                    if(listen(handle, queueSize) >= 0) output = true;
                }
            }
        }

        return output;
    }

    bool ServerSocket::isOpen() const
    {
        return handle > -1;
    }

    void ServerSocket::closePort()
    {
        if (handle > -1)
        {
            close(handle);
            handle = -1;
        }
    }

    int ServerSocket::getHandle() const
    {
        return handle;
    }

    sockaddr_in ServerSocket::getAddress() const
    {
        return address;
    }

    ServerSocket::~ServerSocket()
    {
        closePort();
    }
}