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
    /*******************************************************************************************************************
     * This class represents a raw socket with a dedicated port number. It will only retrieve and send raw data. Any
     * encryption or decryption that needs to be performed will be the sole responsibility of the person using this
     * class. This class will assume no particular protocol, and it will also assume that it's not sharing a port with
     * another socket/application.
     ******************************************************************************************************************/
    class ServerSocket
    {
        int handle, queueSize;
        sockaddr_in address;

    public:

        /***************************************************************************************************************
         * This constructor will set the intended port number for this socket but will take no farther action. The
         * socket will not be opened, no listening will occur at all. We are simply setting the internal state of the
         * object.
         **************************************************************************************************************/
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