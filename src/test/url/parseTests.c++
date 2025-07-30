/*******************************************************
* Created by Cryos on 7/28/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include "PrintHelper.h++"
#include "Url.h++"

using namespace std;

namespace StiltFox::DialUp::Tests::Parse
{
    TEST(parse, will_parse_the_url_path_of_a_well_formed_path_with_parameters)
    {
        //given we have a url path with no host, port, or protocol
        const string urlString = "/fox/foo/bar/baz?pickles=potatoe&eyes=orange";

        //when we parse the url
        const auto actual = Url::parse(urlString);

        //then we get back the parsed url
        Url expected = {"", "", -1, {"fox","foo","bar","baz"},
                     {{"pickles","potatoe"},{"eyes","orange"}}};
        EXPECT_EQ(expected, *actual);
    }

    TEST(parse, will_parse_the_url_path_of_a_well_formed_path_without_parameters)
    {
        //given we have a url path with no host, port, protocol, or parameters
        const string urlString = "/fox/foo/bar/baz";

        //when we parse the url
        const auto actual = Url::parse(urlString);

        //then we get back the parsed url
        const Url expected = {"", "", -1, {"fox","foo","bar","baz"},{}};
        EXPECT_EQ(expected, *actual);
    }

    TEST(parse, will_parse_a_full_url_with_host_protocol_and_port_without_a_path)
    {
        //given we have a url with a host, port, and protocol
        const string urlString = "http://example.com:8080";

        //when we parse the url
        const auto actual = Url::parse(urlString);

        //then we get back the parsed url
        const Url expected = {"http","example.com",8080, {},{}};
        EXPECT_EQ(expected, *actual);
    }

    TEST (parse, will_parse_a_full_url_without_a_port_number_or_path)
    {
        //given we have a url without a port number
        const string urlString = "http://example.com";

        //when we parse the url
        const auto actual = Url::parse(urlString);

        //then we get back the parsed url
        const Url expected = {"http","example.com",-1, {},{}};
        EXPECT_EQ(expected, *actual);
    }

    TEST (parse, will_parse_a_url_with_only_a_host)
    {
        //given we have a url with just a host
        const string urlString = "example.com";

        //when we parse the url
        const auto actual = Url::parse(urlString);

        //then we get back the parsed url
        const Url expected = {"","example.com",-1, {},{}};
        EXPECT_EQ(expected, *actual);
    }

    TEST(parse, will_parse_a_full_url_with_a_path)
    {
        //given we have a url with a path
        const string urlString = "http://example.com:8080/food/order";

        //when we parse the url
        const auto actual = Url::parse(urlString);

        //then we get back the parsed url
        const Url expected = {"http","example.com",8080, {"food","order"},{}};
        EXPECT_EQ(expected, *actual);
    }

    TEST(parse, will_parse_a_full_url_with_a_path_and_query_parameters)
    {
        //given we have a url with all components
        const string urlString = "http://example.com:8080/food/order?user=billybob";

        //when we parse the url
        const auto actual = Url::parse(urlString);

        //then we get back the parsed url
        const Url expected = {"http","example.com",8080,{"food","order"},{{"user","billybob"}}};
        EXPECT_EQ(expected, *actual);
    }

    TEST(parse, will_parse_a_url_with_encoded_characters)
    {
        //given we have a url with escaped characters
        const string urlString = "http://example.com:443/i%20have%20spaces?%2Faq%2A%28%29%24%3F%21%40%23%24%25%5E%26_-%"
                                 "2B%3D=%E6%B0%B4%E3%82%92%E3%81%8F%E3%81%A0%E3%81%95%E3%81%84";

        //when we parse the url
        const auto actual = Url::parse(urlString);

        //then we get back the parsed url
        const Url expected = {"http","example.com",443,{"i have spaces"},
                             {{"/aq*()$?!@#$%^&_-+=","水をください"}}};
        EXPECT_EQ(expected, *actual);
    }

    TEST(parse, will_return_a_null_pointer_when_parsing_an_invalid_hostname)
    {
        //given we have a url with an invalid hostname
        const string urlString = "http://exa mp*(e.com";
        const string urlStringExcaped = "http://exa%20mple.com";

        //when we parse the urls
        const auto actual = Url::parse(urlString);
        const auto actualExcaped = Url::parse(urlStringExcaped);

        //then we get back null pointers
        EXPECT_EQ(nullptr, actual);
        EXPECT_EQ(nullptr, actualExcaped);
    }

    TEST(parse, will_return_a_null_pointer_when_parsing_an_invalid_protocol)
    {
        //given we have a url with a protocol that has invalid characters
        const string invalidProtocol = "ht-1%20p://example.com";

        //when we parse the url
        const auto actual = Url::parse(invalidProtocol);

        //then we get back a null pointer
        EXPECT_EQ(nullptr, actual);
    }
}