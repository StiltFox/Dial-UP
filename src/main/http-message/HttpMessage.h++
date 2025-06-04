#ifndef Stilt_Fox_3482cf09b5224d89bc0997f795fff9e1
#define Stilt_Fox_3482cf09b5224d89bc0997f795fff9e1
#include <queue>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace StiltFox::DialUp
{
    struct HttpMessage
    {
        enum Method {GET,HEAD,POST,PUT,PATCH,DELETE,CONNECT,OPTIONS,TRACE,ERROR,NONE};
        int statusCode = 0;
        Method httpMethod = NONE;
        std::string httpVersion, requestUri, statusReason, body;
        std::unordered_map<std::string,std::vector<std::string>> headers;

        HttpMessage(int statusCode, std::unordered_map<std::string,std::vector<std::string>> headers = {},
            std::string body = "", const std::string& statusReason = "");
        HttpMessage(Method method, std::string uri = "*",
            std::unordered_map<std::string,std::vector<std::string>> headers = {}, std::string body = "");
        HttpMessage(int socketId, const std::function<int(int,char*,int)>& reader);
        HttpMessage(const HttpMessage&);
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
