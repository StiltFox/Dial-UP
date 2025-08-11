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
     * - host - This is the hostname included in the url. This is a string representation, but it could be an IP address
     *          or a domain name.
     * - port - This is the port number in integer form.
     * - pathSegments - This is the path (the part of the url after the first '/') broken into a vector. The segments
     *                  are loaded in the order they appear in the path. EX: https://example.com:8080/test/endpoint
     *                  would be {"test","endpoint"}.
     * - parameters - This is a map of the query parameters that appear at the end of the url after the question mark.
     *                EX: https://example.com:8080/test/endpoint?help=not_here&lost=me would be
     *                {{"help","not_here"},{"lost","me"}}.
     ******************************************************************************************************************/
    struct Url
    {
        std::string protocol,host;
        int port;
        std::vector<std::string> pathSegments;
        std::unordered_map<std::string, std::string> parameters;

        /***************************************************************************************************************
         * This constructor is used to manually put together a url object.
         *
         * @param protocol - this is the communication protocol that is being used. This is the part of the url that
         *                   comes before the "://" part. IE: "http", "https", "ftp", "sftp", etc...
         * @param host - this is the host name or ip address of the computer we wish to connect to. IE: "google.com",
         *               "8.8.8.8", etc...
         * @param port - This is the port number in integer form.
         * @Param pathSegments - This is the path (the part of the url after the first '/') broken into a vector. The
         *                       segments are loaded in the order they appear in the path. EX:
         *                       https://example.com:8080/test/endpoint would be {"test","endpoint"}.
         * @param parameters -
         **************************************************************************************************************/
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