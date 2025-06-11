#ifdef MAC
    #include <sys/types.h>
#endif
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Socket.h++"

using namespace StiltFox::DialUp;

Connection::Connection(int handle)
{
    this->handle = handle;
}

int Connection::getHandle()
{
    return handle;
}

HttpMessage Connection::receiveData()
{
    return HttpMessage(handle, &read);
}

void Connection::sendData(HttpMessage data)
{
    std::string toSend = data.printAsResponse();
    send(handle, toSend.c_str(), toSend.size(), 0);
}