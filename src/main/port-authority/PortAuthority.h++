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
#include "Socket.h++"

namespace StiltFox::DialUp
{
    class PortAuthority
    {
        int maxThreads;
        std::shared_ptr<Socket> socket;

    public:
        EndpointRegistry registry;

        PortAuthority(int portNumber, int maxWorkerThreads);
    };
}
#endif