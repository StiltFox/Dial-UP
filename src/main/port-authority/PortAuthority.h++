/*******************************************************
* Created by cryos on 6/20/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifndef Stilt_Fox_65d185776f6249aaa0182b82cad0384b
#define Stilt_Fox_65d185776f6249aaa0182b82cad0384b
#include <memory>
#include "EndpointRegistry.h++"
#include "ServerSocket.h++"

namespace StiltFox::DialUp
{
    class PortAuthority
    {
        int maxThreads;
        long maxWaitTime, maxDataSize;
        std::shared_ptr<ServerSocket> socket, killSocket;
        static const HttpMessage KILL_MESSAGE;
        std::thread** workers;

        void connectionThreadHandler();
        void listenForKillCommand();

    public:
        EndpointRegistry registry;

        PortAuthority(int portNumber, int killPortNumber, int maxWorkerThreads, long maxWaitTime=10000,
            long maxDataSize=80000000);

        void startApplication();
        void stopApplication();

        ~PortAuthority();
    };
}
#endif