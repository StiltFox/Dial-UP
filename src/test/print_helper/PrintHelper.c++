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
    void PrintTo(const HttpMessage& message, std::ostream* outputStream)
    {
        json output;
        output["status_code"] = message.statusCode;
        output["http"]["method"] = message.getHttpMethodAsString();
        output["http"]["version"] = message.httpVersion;
        output["request"]["uri"] = message.requestUri;
        output["body"] = message.body;
        output["headers"] = message.headers;

        *outputStream << output.dump(4) << endl;
    }
}