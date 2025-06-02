#ifndef StiltFox_UniversalLibrary_Socket
#define StiltFox_UniversalLibrary_Socket
#include <netinet/in.h>
#include "HttpMessage.h++"

namespace StiltFox::DialUp
{
    class Connection
    {
        int handle;

        public:
        Connection(int handle);
        HttpMessage receiveData();
        void sendData(HttpMessage data);
        int getHandle();
        ~Connection();
    };

    class Socket
    {
        int socketHandle;
        int queue;
        sockaddr_in address;
        
        public:
        Socket(int portNumber, int queueSize = 3);
        bool listenPort();
        Connection* openConnection() const;
        int getHandle() const;
        void sendData(HttpMessage data) const;
        void closePort();
        ~Socket();
    };
}

#endif