/*******************************************************
* Created by Cryos on 7/24/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include "Url.h++"

using namespace std;

namespace StiltFox::DialUp::Tests::UrlTests::ToUrl
{
    TEST(toUrl, will_properly_convert_a_url_with_no_queries_to_a_url_string)
    {
        //given we have a url with no query parameters
        const Url noParameters({"big","oof"});

        //when we convert the url to a url string
        const auto actual = noParameters.toUrl();

        //then we get back the proper url string
        EXPECT_EQ("/big/oof", actual);
    }

    TEST(toUrl, will_properly_convert_a_url_with_query_parameters_to_a_url_string)
    {
        //given we have a url with query parameters
        const Url parameterUrl({"big","oof"},{{"param","pickle"},{"param2","cheese"}});

        //when we convert the url to a url string
        const auto actual = parameterUrl.toUrl();

        //then we get back the proper url string
        EXPECT_EQ("/big/oof?param2=cheese&param=pickle", actual);
    }

    TEST(toUrl, will_encode_all_non_ascii_or_non_accepted_character_into_url_percent_encodings)
    {
        //given we have a url with many reserved characters
        const Url encodingNeededUrl({"i have spaces"},{{"/aq*()$?!@#$%^&_-+=","水をください"}});

        //when we convert the url to a url string
        const auto actual = encodingNeededUrl.toUrl();

        //then we get back the proper url string
        EXPECT_EQ("/i%20have%20spaces?%2Faq%2A%28%29%24%3F%21%40%23%24%25%5E%26_-%2B%3D=%E6%B0%B4%E3%82%92%E3%81%8F%E3%"
            "81%A0%E3%81%95%E3%81%84", actual);
    }
}