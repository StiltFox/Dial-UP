/*******************************************************
* Created by Cryos on 6/19/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include "EndpointRegistry.h++"

#include <ranges>

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

    unordered_map<string, unordered_set<HttpMessage::Method>> EndpointRegistry::getRegisteredEndpoints() const
    {
        unordered_map<string, unordered_set<HttpMessage::Method>> output;

        for (const auto& url : endpoints)
            for (const auto &method: url.second | views::keys) output[url.first].emplace(method);

        return output;
    }

    HttpMessage EndpointRegistry::submitMessage(const HttpMessage& message)
    {
        HttpMessage output = {404,{},""};

        if (endpoints.contains(message.requestUri))
        {
            if (endpoints[message.requestUri].contains(message.httpMethod))
            {
                output = endpoints[message.requestUri][message.httpMethod](message);
            }
            else
            {
                output.statusCode = 405;
            }
        }

        return output;
    }
}