/*******************************************************
* Created by Cryos on 7/25/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include "Url.h++"

using namespace std;

namespace StiltFox::DialUp::Tests::UrlTests::ToUrlWithoutParameters
{
    TEST(toUrlWithoutParameters, will_ignore_url_parameters_when_printing_the_url_string)
    {
        //given we have a url with parameters
        const Url urlWithParameters("","",-1,{"path"},{{"param","test"},{"param1","test1"},{"param2","param2"}});

        //when we get the url string without parameters
        auto actual = urlWithParameters.toUrlWithoutParameters();

        //then we get back a url path without the parameters
        EXPECT_EQ("/path", actual);
    }

    TEST(toUrlWithoutParameters, will_url_encode_invalid_characters_into_url_percent_encodings)
    {
        //given we have a url path that needs encoded
        const Url urlThatNeedsEncoded("","",-1,{"/aq*()$?!@#$%^&_-+=","水をください"},{});

        //when we get the url string without parameters
        auto actual = urlThatNeedsEncoded.toUrlWithoutParameters();

        //then we get back the proper url string
        EXPECT_EQ("/%2Faq%2A%28%29%24%3F%21%40%23%24%25%5E%26_-%2B%3D/%E6%B0%B4%E3%82%92%E3%81%8F%E3%81%A0%E3%81%95%E3%"
            "81%84", actual);
    }
}