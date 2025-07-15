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
    /*******************************************************************************************************************
     * Most web frameworks have the idea of an endpoint. An endpoint is where the program accepts an Http request
     * directed at a particular path. For example http://example.com/pickles is one endpoint, and
     * http://example.com/crackers/cheese is another endpoint. The endpoints referring to the part after the hostname
     * of course.
     * In this instance an 'endpoint' can be any function that takes in an HttpMessage, and then returns one.
     ******************************************************************************************************************/
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