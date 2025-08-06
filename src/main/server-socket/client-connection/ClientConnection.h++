/*******************************************************
* Created by cryos on 7/12/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifndef Stilt_Fox_472385557d8d4ecd9fc2985b42cf3025
#define Stilt_Fox_472385557d8d4ecd9fc2985b42cf3025
#include "ServerSocket.h++"
#include "Response.h++"

namespace StiltFox::DialUp
{
    /*******************************************************************************************************************
     * This object represents a connection made to a port. It's called "Client Connection" because you as the server
     * are waiting on a connection from the client. You cannot initiate communication with this class, only wait for and
     * respond to incoming connections.
     ******************************************************************************************************************/
    class ClientConnection
    {
        int handle, socketHandle;
        long maxWaitTimeMs, maxDataSizeBytes;
        sockaddr_in address;

        /***************************************************************************************************************
         * This function will read all data transmitted to the socket, to this connection. It will respect the set
         * maximum wait time and maximum data size. This function is managed internally and should not be called by the
         * user.
         *
         * @param response - this is the destination for the data to go to. If there is an error, this will be indicated
         *                   here also. Check that this data is valid before parsing it. Take anything in this object
         *                   as untrusted and parse defensively.
         * @param holt - this boolean when set will halt the reading process. This can be triggered by many things
         *               including hitting set limits.
         **************************************************************************************************************/
        void readSocketToBuffer(Response& rawData, bool& holt);

    public:
        /***************************************************************************************************************
         * These constructors essentially do the same thing. The Constructor that takes in the ServerSocket object
         * simply gets the socket handle and socket address from the object instead of needing them passed in manually.
         *
         * These constructors setup the connection to be used, but do not block the thread yet. This means you may
         * perform other actions before waiting and listening if you need to.
         *
         * @param serverSocket - The server socket that we are going to wait for a connection from. The socket must be
         *                       open.
         * @param maxWaitTimeMs - The amount of time to wait for a request to finish transmitting before terminating the
         *                        connection.
         * @param maxDataSizeBytes - This is the maximum size of data that we'll allow to be collected before forcefully
         *                           terminating the connection.
         **************************************************************************************************************/
        ClientConnection(const ServerSocket& serverSocket, long maxWaitTimeMs = 10000,
            long maxDataSizeBytes = 80000000);
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