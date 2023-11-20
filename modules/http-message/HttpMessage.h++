#ifndef STILTFOX_DIAL_UP_HTTPMESSAGE_H
#define STILTFOX_DIAL_UP_HTTPMESSAGE_H
#include <string>
#include <unordered_map>
#include <queue>
#include <functional>

namespace StiltFox::DialUp
{
    struct HttpMessage
    {
        enum Method {GET,HEAD,POST,PUT,PATCH,DELETE,CONNECT,OPTIONS,TRACE,ERROR,NONE};
        int statusCode = 0;
        Method httpMethod = NONE;
        std::string requestUri;
        std::string statusReason;
        std::unordered_map<std::string,std::string> headers;
        std::string body;

        HttpMessage(int statusCode, std::unordered_map<std::string,std::string> headers = {}, std::string body = "", const std::string& statusReason = "");
        HttpMessage(Method method, std::string uri = "*", std::unordered_map<std::string,std::string> headers = {}, std::string body = "");
        HttpMessage(int socketId, const std::function<int(int,char*,int)>& reader);
        std::string getHttpMethodAsString() const;
        std::string printAsResponse() const;
        std::string printAsRequest() const;
        bool operator==(const HttpMessage&) const;
        bool operator!=(const HttpMessage&) const;

    protected:
        std::string printBodyAndHeaders() const;
        void parseString(std::queue<char>&);
    };
}
#endif
