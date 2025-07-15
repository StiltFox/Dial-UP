/*******************************************************
* Created by cryos on 6/5/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifndef Stilt_Fox_4329fc21886c468c894897102aaa54e6
#define Stilt_Fox_4329fc21886c468c894897102aaa54e6
#include <memory>
#include <netinet/in.h>
#include "ClientConnection.h++"

namespace StiltFox::DialUp
{
    class ServerSocket
    {
        int handle, queueSize;
        sockaddr_in address;

    public:

        ServerSocket(int portNumber);

        bool openPort(int queueSize = 3);
        bool isOpen() const;
        int getHandle() const;
        sockaddr_in getAddress() const;
        void closePort();
        std::shared_ptr<ClientConnection> createClientConnection();

        ~ServerSocket();
    };
}
#endif