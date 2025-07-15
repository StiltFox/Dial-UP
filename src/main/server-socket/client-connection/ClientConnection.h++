/*******************************************************
* Created by cryos on 7/12/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifndef Stilt_Fox_472385557d8d4ecd9fc2985b42cf3025
#define Stilt_Fox_472385557d8d4ecd9fc2985b42cf3025
#include "Response.h++"

namespace StiltFox::DialUp
{
    class ClientConnection
    {
        int handle, socketHandle;
        long maxWaitTimeMs, maxDataSizeBytes;
        sockaddr_in address;

        void readSocketToBuffer(Response& rawData, bool& holt);

    public:
        ClientConnection(int socketHandle, sockaddr_in address, long maxWaitTimeMs = 10000,
            long maxDataSizeBytes = 80000000);

        Response receiveData();
        void sendData(std::vector<char> data);
        void terminate();
        int getHandle();

        ~ClientConnection();
    };
}
#endif