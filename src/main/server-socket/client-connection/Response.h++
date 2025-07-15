/*******************************************************
* Created by cryos on 7/12/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifndef Stilt_Fox_0c65c7af84804aeea421426acc15f812
#define Stilt_Fox_0c65c7af84804aeea421426acc15f812
#include <vector>
#include <string>

namespace StiltFox::DialUp
{
    struct Response
    {
        std::vector<char> data;
        std::string errorMessage;
    };
}
#endif