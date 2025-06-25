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

    void PortAuthority::connectionThreadHandler()
    {
        auto connection = Socket::Connection::openConnection(*socket, maxWaitTime, maxDataSize);
        auto request = connection->listen();

        if (request.errorMessage == "")
        {
            HttpMessage message = request.data;
        }
        else
        {
            int errorStatus = 0;

            switch (request.errorMessage)
            {
            case "Connection timed out":
                errorStatus = 408;
                break;
            case "Data received exceeds limit":
                errorStatus = 413;
                break;
            default:
                errorStatus = 400;
                break;
            }

            HttpMessage response {errorStatus, {}, ""};

            connection->sendData(response.printAsResponse());
        }
    }

}