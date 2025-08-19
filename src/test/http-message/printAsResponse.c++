/*******************************************************
* Created by Cryos on 7/17/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include "PrintHelper.h++"

using namespace std;

namespace StiltFox::DialUp::Tests::HttpMessageTests::PrintAsResponse
{
    TEST(printAsResponse, will_print_a_well_formatted_http_response)
    {
        //given we have a http message with all the appropriate data for a http response
        const HttpMessage response(200,{{"snacks",{"pickles","cheese"}}},
            "bring some extra smores to the campsite");

        //when we print the message as a Http Response
        const auto actual = response.printAsResponse();

        //then we get back a well formatted http response
        const string expected =
            "HTTP/1.1 200 OK\r\n"
            "snacks: pickles,cheese\r\n\r\n"
            "bring some extra smores to the campsite";
        EXPECT_EQ(expected,actual);
    }

    TEST(printAsResponse, will_not_print_the_http_method_even_if_its_set)
    {
        //given we have a http message with all the appropriate data for a http response with extra data
        HttpMessage response(200,{{"snacks",{"pickles","cheese"}}},
            "bring some extra smores to the campsite");
        response.httpMethod = HttpMessage::Method::PUT;

        //when we print the message as a Http Response
        const auto actual = response.printAsResponse();

        //then we get back a well formatted http response
        const string expected =
            "HTTP/1.1 200 OK\r\n"
            "snacks: pickles,cheese\r\n\r\n"
            "bring some extra smores to the campsite";
        EXPECT_EQ(expected,actual);
    }

    TEST(printAsResponse, will_print_a_well_formatted_response_if_no_headers_are_provided)
    {
        //given we have a http response without headers
        const HttpMessage response(418,{},"I'll be with you in the dark, with your heart as the ark");

        //when we print the message as a http response
        const auto actual = response.printAsResponse();

        //then we get back a well formatted response
        const string expected =
            "HTTP/1.1 418 I'm a teapot\r\n\r\n"
            "I'll be with you in the dark, with your heart as the ark";
        EXPECT_EQ(expected,actual);
    }

    TEST(printAsResponse, will_print_a_well_formatted_response_if_no_body_is_provided)
    {
        //given we have a http response without a body
        const HttpMessage response(500,{{"sentiment",{"hopeful"}}});

        //when we print the message as a http response
        const auto actual = response.printAsResponse();

        //then we get back a well formatted response
        const string expected =
            "HTTP/1.1 500 Internal Server Error\r\n"
            "sentiment: hopeful\r\n\r\n";
        EXPECT_EQ(expected,actual);
    }

    TEST(printAsResponse, will_print_a_well_formatted_response_if_no_headers_or_body_is_provided)
    {
        //given we have a http response without a body or headers
        const HttpMessage response(500);

        //when we print the message as a http response
        const auto actual = response.printAsResponse();

        //then we get back a well formatted response
        const string expected =
            "HTTP/1.1 500 Internal Server Error\r\n\r\n";
        EXPECT_EQ(expected,actual);
    }
}