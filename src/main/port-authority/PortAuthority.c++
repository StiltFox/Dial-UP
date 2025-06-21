/*******************************************************
* Created by Cryos on 6/20/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include "PortAuthority.h++"

using namespace std;

namespace StiltFox::DialUp
{
    PortAuthority::PortAuthority(int portNumber, int maxWorkerThreads)
    {
        socket = make_shared<Socket>(portNumber);
        maxThreads = maxWorkerThreads;
    }
}