/*******************************************************
* Created by Cryos on 6/19/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include "EndpointRegistry.h++"
#include <stack>
#include <ranges>

using namespace std;

namespace StiltFox::DialUp
{
    void EndpointRegistry::registerEndpoint(
            const string& url, const HttpMessage::Method& method, const Endpoint& endpoint
        )
    {
        registerEndpoint(Url::parse(url), method, endpoint);
    }

    void EndpointRegistry::registerEndpoint(const Url& url, const HttpMessage::Method& method, const Endpoint& endpoint)
    {
        TreeNode* currentNode = &root;

        for (const auto& segment : url.pathSegments)
        {
            if (!currentNode->children.contains(segment)) currentNode->children[segment];
            currentNode = &currentNode->children[segment];
        }

        currentNode->endpoints[method] = endpoint;
    }

    void EndpointRegistry::unregisterEndpoint(const string& url, const HttpMessage::Method& method)
    {
        unregisterEndpoint(Url::parse(url), method);
    }

    void EndpointRegistry::unregisterEndpoint(const Url& url, const HttpMessage::Method& method)
    {
        bool endpointFound = true;
        TreeNode* currentNode = &root;
        stack<string> segmentStack;

        for (const auto& segment : url.pathSegments)
        {
            if (currentNode->children.contains(segment))
            {
                currentNode = &currentNode->children[segment];
                segmentStack.push(segment);
            }
            else
            {
                endpointFound = false;
                break;
            }
        }

        currentNode->endpoints.erase(method);

        if (endpointFound)
        {
            while (!segmentStack.empty() && currentNode->parent != nullptr)
            {
                if (currentNode->children.empty() && currentNode->endpoints.empty())
                {
                    currentNode->parent->children.erase(segmentStack.top());
                    segmentStack.pop();
                    currentNode = currentNode->parent;
                }
                else
                {
                    break;
                }
            }
        }
    }

    void EndpointRegistry::processNode(Url currentPath, const TreeNode& currentNode,
                                       unordered_map<string, unordered_set<HttpMessage::Method>>& nodeMap) const
    {
        if (!currentNode.endpoints.empty())
        {
            for (const auto& endpoint : currentNode.endpoints) nodeMap[currentPath.toUrl()].insert(endpoint.first);
        }

        if (!currentNode.children.empty())
        {
            for (const auto& child : currentNode.children)
            {
                Url childPath = currentPath;
                childPath.pathSegments.push_back(child.first);
                processNode(childPath, child.second, nodeMap);
            }
        }
    }

    unordered_map<string, unordered_set<HttpMessage::Method>> EndpointRegistry::getRegisteredEndpoints() const
    {
        unordered_map<string, unordered_set<HttpMessage::Method>> output;

        Url startUrl;
        processNode(startUrl, root, output);

        return output;
    }

    HttpMessage EndpointRegistry::submitMessage(const HttpMessage& message)
    {
        HttpMessage output = {404,{},""};

        TreeNode* currentNode = &root;
        for (const auto& segment : message.requestUri.pathSegments)
        {
            if (currentNode->children.contains(segment))
            {
                currentNode = &currentNode->children[segment];
            }
            else if (currentNode->children.contains("*"))
            {
                currentNode = &currentNode->children["*"];
            }
            else
            {
                currentNode = nullptr;
                break;
            }
        }

        if (currentNode != nullptr)
        {
            if (currentNode->endpoints.contains(message.httpMethod))
            {
                output = currentNode->endpoints[message.httpMethod](message);
            }
            else
            {
                output.statusCode = 405;
            }
        }

        return output;
    }
}