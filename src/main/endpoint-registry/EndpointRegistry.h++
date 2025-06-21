/*******************************************************
* Created by cryos on 6/19/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifndef Stilt_Fox_21e63198d1504c67a2a82a4ad7a0d29c
#define Stilt_Fox_21e63198d1504c67a2a82a4ad7a0d29c
#include <functional>
#include "HttpMessage.h++"

namespace StiltFox::DialUp
{
    typedef std::function<HttpMessage(HttpMessage)> Endpoint;

    class EndpointRegistry
    {
        std::unordered_map<std::string, std::unordered_map<HttpMessage::Method, Endpoint>> endpoints;

    public:
        void registerEndpoint(const std::string& url, const HttpMessage::Method& method, const Endpoint& endpoint);
        void unregisterEndpoint(const std::string& url, const HttpMessage::Method& method);

        HttpMessage submitMessage(const HttpMessage& message);
    };
}
#endif