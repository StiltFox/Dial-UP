/*******************************************************
* Created by cryos on 6/19/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifndef Stilt_Fox_21e63198d1504c67a2a82a4ad7a0d29c
#define Stilt_Fox_21e63198d1504c67a2a82a4ad7a0d29c
#include <memory>
#include <functional>
#include <unordered_set>
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

    /*******************************************************************************************************************
     * This class can be thought of as similar to a router. It takes in a message and based on it's http method and
     * url, routs it to the appropriate function.
     ******************************************************************************************************************/
    class EndpointRegistry
    {
        /***************************************************************************************************************
         * This private internal class is used to keep track of url endpoints. It treats each url as a path through a
         * tree from root.
         *
         * Member variables:
         * - parent: a pointer to the parent node. If this is blank it is assumed that this is the root
         * - children: a map of pointers to the different child nodes. this is intended to take a segment of a url
         *             path as a key, for easy and quick navication.
         * - endpoints: This matches an Http Method to a function that handles it. If this url path supports a method
         *              it will be in this map.
         **************************************************************************************************************/
        struct TreeNode
        {
            std::shared_ptr<TreeNode> parent = nullptr;
            std::unordered_map<std::string, std::shared_ptr<TreeNode>> children;
            std::unordered_map<HttpMessage::Method, Endpoint> endpoints;
        };

        std::shared_ptr<TreeNode> root = std::make_shared<TreeNode>();

        /***************************************************************************************************************
         * This function is used to process tree nodes back into URL strings.
         * This is an internal function and should not be called by the user. Just noting what it does for completeness.
         *
         * @param currentPath - this is the url object with the current path representation up and to this point. This
         *                      is meant to be a starting point, onto which each node will add it's own path segment. If
         *                      starting from root this should be a new object with no data.
         * @param currentNode - The current node of the url tree that we are on. This function works recursivly and will
         *                      traverse down the whole tree from this point. It's best if you put the root node here.
         * @param nodeMap - This is where the information will be output to. Because this is recursive a return value
         *                  did not easily provide the desired functionality. Please pardon this break from traditional
         *                  form.
         **************************************************************************************************************/
        void processNode(Url currentPath, const TreeNode& currentNode,
                         std::unordered_map<std::string,std::unordered_set<HttpMessage::Method>>& nodeMap) const;
    public:
        /**************************************************************************************************************
         * Adds an endpoint to the registry so that it can be routed to. If you want a function to handle more than one
         * url or http method, you must register that function for each url/method combination that you wish to support.
         *
         * You may only have one entry per url/method pair. If you try to register a function again for the same
         * url/method pair, the old entry will be overwritten.
         *
         * @param url - A string representing the URL path for the function. This is only the bit that comes after
         *              the host name. ex: "/pickles" would route from "http://localhost/pickles".
         * @param method - This is the http method that this url supports. IE: put, get, delete, post, ect...
         * @param endpoint - The function that handles the request.
         *************************************************************************************************************/
        void registerEndpoint(const std::string& url, const HttpMessage::Method& method, const Endpoint& endpoint);
        /**************************************************************************************************************
         * Adds an endpoint to the registry so that it can be routed to. If you want a function to handle more than one
         * url or http method, you must register that function for each url/method combination that you wish to support.
         *
         * You may only have one entry per url/method pair. If you try to register a function again for the same
         * url/method pair, the old entry will be overwritten.
         *
         * @param url - A url object representing the URL path for the function. This is only the bit that comes after
         *              the host name. ex: "/pickles" would route from "http://localhost/pickles".
         * @param method - This is the http method that this url supports. IE: put, get, delete, post, ect...
         * @param endpoint - The function that handles the request.
         *************************************************************************************************************/
        void registerEndpoint(const Url& url, const HttpMessage::Method& method, const Endpoint& endpoint);
        /***************************************************************************************************************
         * This method will remove a function from the registry and it will no longer be processed. No memory will be
         * deleted, so if you passed anything that requires deallocation do it yourself.
         *
         * @param url - A string representing the URL path for the function that you wish to remove. This is only the
         *              bit that comes after the host name. ex: "/pickles" would route from "http://localhost/pickles".
         * @param method - The http method for which to remove the function. Please note that if a url has more than one
         *                 method mapping, then rest will remmain registered. IE: deleting a GET handler will not also
         *                 remove your POST handler.
         **************************************************************************************************************/
        void unregisterEndpoint(const std::string& url, const HttpMessage::Method& method);
        /***************************************************************************************************************
         * This method will remove a function from the registry and it will no longer be processed. No memory will be
         * deleted, so if you passed anything that requires deallocation do it yourself.
         *
         * @param url - A url object representing the URL path for the function that you wish to remove. This is only
         *              the bit that comes after the host name. ex: "/pickles" would route from
         *              "http://localhost/pickles".
         * @param method - The http method for which to remove the function. Please note that if a url has more than one
         *                 method mapping, then rest will remmain registered. IE: deleting a GET handler will not also
         *                 remove your POST handler.
         **************************************************************************************************************/
        void unregisterEndpoint(const Url& url, const HttpMessage::Method& method);

        /***************************************************************************************************************
         * This method will return a map of all endpoints an each method they support. While we'd love to provide the
         * function handler as well, the std::function object makes retrieving this consistently impossible, especially
         * when lambdas are envolved.
         *
         * @return A map of all endpoints.
         **************************************************************************************************************/
        std::unordered_map<std::string,std::unordered_set<HttpMessage::Method>> getRegisteredEndpoints() const;
        /***************************************************************************************************************
         * Submit an Http Message for processing!!!!!
         * This method will look at the Http method and request URI of the message, then pass it to the proper
         * processor.
         *
         * @param message - The message to be processed.
         * @return The response from the processor. If no processor is found a generic 404 will be returned.
         **************************************************************************************************************/
        HttpMessage submitMessage(const HttpMessage& message) const;
    };
}
#endif