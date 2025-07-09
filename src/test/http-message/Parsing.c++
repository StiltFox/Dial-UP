/*******************************************************
* Created by Cryos on 7/6/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include "PrintHelper.h++"

using namespace std;

namespace StiltFox::DialUp
{
    inline vector<char> stringToVector(const string& str)
    {
        return {str.begin(), str.end()};
    }

    TEST(Constructor, will_parse_a_properly_formatted_response)
    {
        //given we have a properly formatted http response
        const string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n"
            "body text";

        //when we parse this to be a Http response
        const HttpMessage actual(stringToVector(response));

        //then we get back the message parsed properly
        const HttpMessage expected =
            {200,{{"Content-Type",{"text/html"}},{"Connection",{"close"}}},"body text"};
        EXPECT_EQ(expected, actual);
    }

    TEST(Constructor, will_parse_a_properly_formatted_request)
    {
        //given we have a properly formatted http request
        const string request =
            "POST /test/path HTTP/1.1\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n"
            "body text";

        //when we parse the http request
        const HttpMessage actual(stringToVector(request));

        //then we get back the message parsed properly
        const HttpMessage expected = {HttpMessage::POST,"/test/path",
            {{"Content-Type",{"text/html"}},{"Connection",{"close"}}}, "body text"};
        EXPECT_EQ(expected, actual);
    }

    TEST(Constructor, will_ignore_additional_whitespace_at_the_beginning_of_an_http_message)
    {
        //given we have a properly formatted http request with extra spaces at the beginning
        const string request =
            "   \t\r\t\n \n \t  "
            "POST /test/path HTTP/1.1\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n"
            "body text";

        //when we parse the http request
        const HttpMessage actual(stringToVector(request));

        //then we get back the message parsed properly
        const HttpMessage expected = {HttpMessage::POST,"/test/path",
            {{"Content-Type",{"text/html"}},{"Connection",{"close"}}}, "body text"};
        EXPECT_EQ(expected, actual);
    }

    TEST(Constructor, will_put_multiple_headers_with_the_same_name_in_a_list_together)
    {
        //given we have a properly formatted http request duplicated headers
        const string request =
            "POST /test/path HTTP/1.1\r\n"
            "test: bingo\r\n"
            "test: pickle\r\n"
            "\r\n"
            "body text";

        //when we parse the http request
        const HttpMessage actual(stringToVector(request));

        //then we get back the message parsed properly
        const HttpMessage expected = {HttpMessage::POST,"/test/path",{{"test",{"bingo","pickle"}}},
            "body text"};
        EXPECT_EQ(expected, actual);
    }

    TEST(Constructor, will_parse_comma_sepearated_lists_in_headers)
    {
        //given we have a properly formatted http request with headers with list values
        const string request =
            "POST /test/path HTTP/1.1\r\n"
            "test: bingo,paddle board, pepperoni\r\n"
            "test: pickle\r\n"
            "\r\n"
            "body text";

        //when we parse the http request
        const HttpMessage actual(stringToVector(request));

        //then we get back the message parsed properly
        const HttpMessage expected = {HttpMessage::POST,"/test/path",
            {{"test",{"bingo","paddle board","pepperoni","pickle"}}},"body text"};
        EXPECT_EQ(expected, actual);
    }
}