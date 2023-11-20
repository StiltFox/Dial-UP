#ifndef StiltFox_UniversalLibrary_Socket
#define StiltFox_UniversalLibrary_Socket
#include <netinet/in.h>
#include <Stilt_Fox/UniversalLibrary/HttpMessage.hpp>

namespace StiltFox
{
    namespace Web
    {
        namespace Framework
        {
            class Connection
            {
                int handle;

                public:
                Connection(int handle);
                UniversalLibrary::HttpMessage receiveData();
                void sendData(UniversalLibrary::HttpMessage data);
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
                Connection* openConnection();
                int getHandle();
                void sendData(UniversalLibrary::HttpMessage data);
                void closePort();
                ~Socket();
            };
        }
    }
}
#endif