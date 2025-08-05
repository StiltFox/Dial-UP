/*******************************************************
* Created by cryos on 7/6/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifndef Stilt_Fox_7f4a688829cc4090b3909f996348bf6c
#define Stilt_Fox_7f4a688829cc4090b3909f996348bf6c
#include <nlohmann/json.hpp>
#include "HttpMessage.h++"
#include "Url.h++"

namespace StiltFox::DialUp
{
    void PrintTo(const HttpMessage& message, std::ostream* outputStream);
    void PrintTo(const Url& url, std::ostream* outputStream);

    void to_json(nlohmann::json& j, const HttpMessage& message);
    void to_json(nlohmann::json& j, const Url& url);
}

#endif