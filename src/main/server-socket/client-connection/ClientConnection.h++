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
         * This constructor is a convenience constructor that simplifies the setup process by taking in a ServerSocket
         * object and extracting the necessary information itself. This constructor will set up a connection that can
         * be used to listen for incoming network traffic on a port. This constructor does not block the thread, and you
         * may do other actions before waiting for a connection if you'd like.
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
        /***************************************************************************************************************
         * This constructor takes in a socket handle and address then uses that information to set up a connection that
         * can be used with the provided socket handle. This constructor does not block the thread, and you may do
         * other actions before waiting for a connection if you'd like.
         *
         * @param socketHandle - An integer value that refers to a socket handle/file descriptor. This is used to tell
         *                       the operating system what port we're listening to.
         * @param address - A struct defined by the standard socket headers that come with unix based systems. This
         *                  contains information about how the socket operates, what port number it listens to and other
         *                  important information. It's best that you do not fill this out yourself. Use the value
         *                  in the ServerSocket object after you've instantiated it. This will ensure that the
         *                  information provided aligns with what the socket was created with.
         * @param maxWaitTimeMs - The amount of time to wait for a request to finish transmitting before terminating the
         *                        connection.
         * @param maxDataSizeBytes - This is the maximum size of data that we'll allow to be collected before forcefully
         *                           terminating the connection.
         **************************************************************************************************************/
        ClientConnection(int socketHandle, sockaddr_in address, long maxWaitTimeMs = 10000,
            long maxDataSizeBytes = 80000000);

        /***************************************************************************************************************
         * This function will block the thread and wait for incoming network traffic. Then based on set values like
         * 'maxWaitTimeMs' it will retrieve data while checking for error or failure conditions. This data will be
         * returned as a raw array of chars. This data is untyped, and you should parse it out into whatever format you
         * wish before using it in your program. Validation is your responsibility.
         *
         * @return A response object that represents the data received. Data that comes in will be represented as a
         *         vector of type char. This is raw binary data and should be treated as such. If any errors occur, the
         *         errorMessage field will be set. This will contain a description of what went wrong. Partial data may
         *         still be present in the data field. Do with this what you will. If the errorMessage field is set to
         *         an empty string, it can be assumed that nothing went wrong during data retrieval.
         **************************************************************************************************************/
        Response receiveData();
        /***************************************************************************************************************
         * This function is used to send a response to the client once it's connected with you. receiveData should
         * always be called before using this function, as if you don't have an active connection nothing will happen.
         *
         * @param data - This represents a raw set of binary data that you wish to send back to the client. The client
         *               can choose to interpret this however they want.
         **************************************************************************************************************/
        void sendData(std::vector<char> data);
        /***************************************************************************************************************
         * This function will terminate the connection. No more data can be sent or received until receiveData is called
         * again.
         **************************************************************************************************************/
        void terminate();
        /***************************************************************************************************************
         * Gets the handle of the current connection. This will only be a valid handle if receiveData has been called
         * and a connection has been established. Terminating the connection will make this return an invalid handle
         * again.
         *
         * @return The handle for the client connection if it is valid. -1 will be returned if there's an issue.
         **************************************************************************************************************/
        int getHandle();

        /***************************************************************************************************************
         * This deconstructor will terminate the connection upon scoping out or being deleted.
         **************************************************************************************************************/
        ~ClientConnection();
    };
}
#endif