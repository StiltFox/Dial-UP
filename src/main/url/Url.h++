/*******************************************************
* Created by cryos on 7/22/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifndef Stilt_Fox_6fed102348ef4c75aa4e588e1d241356
#define Stilt_Fox_6fed102348ef4c75aa4e588e1d241356
#include <string>
#include <vector>
#include <unordered_map>

namespace StiltFox::DialUp
{
    /*******************************************************************************************************************
     * This struct is used to represent a url in a way that's easy to route. This also makes it easy to get path
     * variables and queries parameters that you need.
     *
     * - protocol - This is a string representation of the communication protocol used. IE: Http, Https, ftp, ect...
     * - host - This is the hostname included in the url. This is a string representation but it could be an IP address
     *          or a domain name.
     ******************************************************************************************************************/
    struct Url
    {
        std::string protocol,host;
        int port;
        std::vector<std::string> pathSegments;
        std::unordered_map<std::string, std::string> parameters;

        Url(std::string protocol = "", std::string host = "", int port = -1, std::vector<std::string> pathSegments = {},
            std::unordered_map<std::string, std::string> parameters = {});

        std::string toUrl() const;
        std::string toUrlWithoutParameters() const;
        static Url parse(const std::string& url);

        bool operator==(const Url& other) const;
        bool operator!=(const Url& other) const;
        bool operator==(const std::string& other) const;
        bool operator!=(const std::string& other) const;
    };
}
#endif