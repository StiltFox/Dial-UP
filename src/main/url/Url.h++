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
#include <memory>
#include <unordered_map>

namespace StiltFox::DialUp
{
    struct Url
    {
        std::string protocol,host;
        int port;
        std::vector<std::string> pathSegments;
        std::unordered_map<std::string, std::string> parameters;

        std::string toUrl() const;
        std::string toUrlWithoutParameters() const;
        static std::shared_ptr<Url> parse(const std::string& url) ;

        bool operator==(const Url& other) const;
        bool operator!=(const Url& other) const;
        bool operator==(const std::string& other) const;
        bool operator!=(const std::string& other) const;
    };
}
#endif