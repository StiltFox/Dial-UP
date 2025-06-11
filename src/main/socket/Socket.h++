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
#include "HttpMessage.h++"

namespace StiltFox::DialUp::Http
{
    class Connection
    {
        int socketHandle, connectionHandle, queue;
        sockaddr_in address;
        Connection();
        void initializeValues(int portNumber, int queueSize);
        bool openSocket();

    public:
        static std::shared_ptr<Connection> openConnection(int portNumber, int queueSize = 3);
        HttpMessage receiveData(long maxBytes, long maxWaitTime) const;
        void sendData(HttpMessage data) const;
        ~Connection();
    };
}
#endif