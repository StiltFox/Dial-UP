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

namespace StiltFox::DialUp
{
    class Socket
    {
        int handle, queueSize;
        sockaddr_in address;

    public:
        Socket(int portNumber);
        bool openPort(int queueSize = 3);
        bool isOpen() const;
        void closePort();
        ~Socket();

        class Connection
        {
            int handle;
            long maxWaitTimeMS, maxDataSizeBytes;
            Connection(int handle, long maxWaitTimeMS, long maxDataSizeBytes);

        public:
            struct Response
            {
                std::vector<char> data;
                std::string errorMessage;
            };

            static std::shared_ptr<Connection> openConnection(
                    const Socket& socket, long maxWaitTimeMS, long maxDataSizeBytes);
            Response listen();
            void sendData(const std::vector<char>& data) const;
            void sendData(const std::string& data) const;
            ~Connection();

        private:
            void readSocketToBuffer(Response& rawData, bool& holt);
        };
    };
}
#endif