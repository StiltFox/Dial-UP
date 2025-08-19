/*******************************************************
* Created by Cryos on 7/6/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include "PrintHelper.h++"
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

namespace StiltFox::DialUp
{
    void to_json(json& j, const Url& url)
    {
        j["protocol"] = url.protocol;
        j["host"] = url.host;
        j["port"] = url.port;
        j["path-segments"] = url.pathSegments;
        j["parameters"] = url.parameters;
    }

    void to_json(json& j, const HttpMessage& message)
    {
        j["status_code"] = message.statusCode;
        j["http"]["method"] = message.getHttpMethodAsString();
        j["http"]["version"] = message.httpVersion;
        j["request"]["uri"] = message.requestUri;
        j["body"] = message.body;
        j["headers"] = message.headers;
    }

    void PrintTo(const HttpMessage& message, std::ostream* outputStream)
    {
        const json output = message;
        *outputStream << output.dump(4) << endl;
    }

    void PrintTo(const Url& url, std::ostream* outputStream)
    {
        const json output = url;
        *outputStream << output.dump(4) << endl;
    }
}