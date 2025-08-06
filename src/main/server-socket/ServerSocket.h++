/*******************************************************
* Created by cryos on 6/5/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifndef Stilt_Fox_4329fc21886c468c894897102aaa54e6
#define Stilt_Fox_4329fc21886c468c894897102aaa54e6
#include <netinet/in.h>

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

        /***************************************************************************************************************
         * This function will attempt to open up the socket and bind it to the requested port number. This function must
         * be called first, before using any other function of this object. If the socket is not open the other
         * functions will return default values.
         *
         * @param queueSize - This is the number of requests for connection that can sit in waiting. Once the socket is
         *                    full, new connections will be met with an error or ignored depending on the underlying
         *                    implementation.
         * @return A boolean value that indicates weather the port was successfully opened or not.
         **************************************************************************************************************/
        bool openPort(int queueSize = 3);
        /***************************************************************************************************************
         * This function returns weather the socket pointed to by this object is open and listening.
         *
         * @return A boolean value that indicates weather or not this socket is ready to receive data.
         **************************************************************************************************************/
        bool isOpen() const;
        /***************************************************************************************************************
         * Gets the handle/file descriptor of the socket that's currently running. This is necessary to call various
         * networking functions. You will typically not need to use this, however it's used internally in other classes
         * that consume this object.
         *
         * @return An integer representation of the socket handle id/file descriptor if the socket is open and
         *         listening. This will return -1 if the socket is not open for some reason.
         **************************************************************************************************************/
        int getHandle() const;
        /***************************************************************************************************************
         * This function gets an object that contains information that was used, or will be used to open the port with
         * the underlying operating system. This is here for inspection mostly, however other classes that consume this
         * may use this information to listen to this socket.
         *
         * @return A struct with address information for the port.
         **************************************************************************************************************/
        sockaddr_in getAddress() const;
        /***************************************************************************************************************
         * This function closes the socket entirely. All currently listening connections will be terminated. Use this
         * to shutdown the network activity. Closing an already closed port produces no adverse side effects as this
         * function does the proper checks.
         **************************************************************************************************************/
        void closePort();

        /***************************************************************************************************************
         * This deconstructor closes the socket upon leaving scope or being deleted.
         **************************************************************************************************************/
        ~ServerSocket();
    };
}
#endif