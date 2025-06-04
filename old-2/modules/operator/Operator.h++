#ifndef STILTFOX_WEB_FRAMEWORK_OPERATOR_H
#define STILTFOX_WEB_FRAMEWORK_OPERATOR_H
#include <functional>
#include <string>
#include "HttpMessage.h++"

namespace StiltFox::DialUp
{
    class Operator
    {
    public:
        void launch(int portNumber = 8080);
        void registerController(std::string path, HttpMessage::Method method, std::function<HttpMessage(HttpMessage)> controller);
    };
}

#endif
