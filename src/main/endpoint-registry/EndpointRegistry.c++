/*******************************************************
* Created by Cryos on 6/19/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include "EndpointRegistry.h++"

using namespace std;

namespace StiltFox::DialUp
{
    void EndpointRegistry::registerEndpoint(
            const string& url, const HttpMessage::Method& method, const Endpoint& endpoint
        )
    {
        endpoints[url][method] = endpoint;
    }

    void EndpointRegistry::unregisterEndpoint(const string& url, const HttpMessage::Method& method)
    {
        if (endpoints.contains(url) && endpoints[url].contains(method))
        {
            endpoints[url].erase(method);
            if (endpoints[url].empty()) endpoints.erase(url);
        }
    }

    HttpMessage EndpointRegistry::submitMessage(const HttpMessage& message)
    {
        HttpMessage output = {404, {}, "", "Not Found"};

        if (endpoints.contains(message.requestUri))
        {
            if (endpoints[message.requestUri].contains(message.httpMethod))
            {
                output = endpoints[message.requestUri][message.httpMethod](message);
            }
            else
            {
                output.statusCode = 405;
                output.statusReason = "Method Not Allowed";
            }
        }

        return output;
    }
}