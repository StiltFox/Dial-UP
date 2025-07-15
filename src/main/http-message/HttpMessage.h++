/*******************************************************
* Created by cryos on 6/5/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/

#ifndef Stilt_Fox_3482cf09b5224d89bc0997f795fff9e1
#define Stilt_Fox_3482cf09b5224d89bc0997f795fff9e1
#include <string>
#include <vector>
#include <unordered_map>

namespace StiltFox::DialUp
{
    /******************************************************************************************************************
     * This class is meant to contain a Http Message. This represents both requests and responses in one class because
     * they're both so similar.
     *****************************************************************************************************************/
    struct HttpMessage
    {
        enum Method {GET,HEAD,POST,PUT,PATCH,DELETE,CONNECT,OPTIONS,TRACE,ERROR,NONE};
        int statusCode = 0;
        Method httpMethod = NONE;
        std::string httpVersion, requestUri, body;
        std::unordered_map<std::string,std::vector<std::string>> headers;

        HttpMessage(const std::vector<char>& data);
        HttpMessage(int statusCode, std::unordered_map<std::string,std::vector<std::string>> headers = {},
            std::string body = "");
        HttpMessage(Method method, std::string uri = "*",
            std::unordered_map<std::string,std::vector<std::string>> headers = {}, std::string body = "");
        HttpMessage(const HttpMessage&);
        std::string getHttpMethodAsString() const;
        std::string printAsResponse() const;
        std::string printAsRequest() const;
        bool operator==(const HttpMessage&) const;
        bool operator!=(const HttpMessage&) const;

    protected:
        std::string printBodyAndHeaders() const;
        void parse(const std::vector<char>& data);
    };
}

#endif
