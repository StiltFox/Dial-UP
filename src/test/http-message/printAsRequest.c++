/*******************************************************
* Created by Cryos on 7/18/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include "PrintHelper.h++"

using namespace std;

namespace StiltFox::DialUp::Tests::HttpMessageTests::PrintAsRequest
{
    TEST(printAsRequest, will_print_a_well_formatted_http_request)
    {
        //given we have a http message with all the appropriate data for a http request
        const HttpMessage request(HttpMessage::Method::GET,"/path",{{"snacks",{"pickles","cheese"}}},
            "bring some extra smores to the campsite");

        //when we print the message as a Http Request
        const auto actual = request.printAsRequest();

        //then we get back a well formatted http request
        const string expected =
            "GET /path HTTP/1.1\r\n"
            "snacks: pickles,cheese\r\n\r\n"
            "bring some extra smores to the campsite";
        EXPECT_EQ(expected,actual);
    }

    TEST(printAsRequest, will_not_print_the_status_code_even_if_its_set)
    {
        //given we have a http message with all the appropriate data for a http request with extra data
        HttpMessage request(HttpMessage::Method::GET,"/path",{{"snacks",{"pickles","cheese"}}},
            "bring some extra smores to the campsite");
        request.statusCode = 200;

        //when we print the message as a Http Request
        const auto actual = request.printAsRequest();

        //then we get back a well formatted http request
        const string expected =
            "GET /path HTTP/1.1\r\n"
            "snacks: pickles,cheese\r\n\r\n"
            "bring some extra smores to the campsite";
        EXPECT_EQ(expected,actual);
    }

    TEST(printAsRequest, will_print_a_well_formatted_request_if_no_headers_are_provided)
    {
        //given we have a http request without headers
        const HttpMessage request(HttpMessage::PUT,"/endpoint",{},
            "I'll be with you in the dark, with your heart as the ark");

        //when we print the message as a http request
        const auto actual = request.printAsRequest();

        //then we get back a well formatted request
        const string expected =
            "PUT /endpoint HTTP/1.1\r\n\r\n"
            "I'll be with you in the dark, with your heart as the ark";
        EXPECT_EQ(expected,actual);
    }

    TEST(printAsRequest, will_print_a_well_formatted_request_if_no_body_is_provided)
    {
        //given we have a http request without a body
        const HttpMessage request(HttpMessage::PATCH,"/endpoint",{{"sentiment",{"hopeful"}}});

        //when we print the message as a http request
        const auto actual = request.printAsRequest();

        //then we get back a well formatted request
        const string expected =
            "PATCH /endpoint HTTP/1.1\r\n"
            "sentiment: hopeful\r\n\r\n";
        EXPECT_EQ(expected,actual);
    }

    TEST(printAsRequest, will_print_a_well_formatted_request_if_no_headers_or_body_is_provided)
    {
        //given we have a http request without a body or headers
        const HttpMessage request(HttpMessage::DELETE);

        //when we print the message as a http response
        const auto actual = request.printAsRequest();

        //then we get back a well formatted response
        const string expected =
            "DELETE / HTTP/1.1\r\n\r\n";
        EXPECT_EQ(expected,actual);
    }
}