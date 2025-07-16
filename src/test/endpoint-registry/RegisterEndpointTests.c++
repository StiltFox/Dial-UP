/*******************************************************
* Created by cryos on 7/16/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include <functional>
#include "PrintHelper.h++"
#include "EndpointRegistry.h++"

using namespace std;

namespace StiltFox::DialUp::Tests::EndpointRegistryTests::RegisterEndpoint
{
    TEST(registerEndpoint, will_overrite_the_registerd_function_if_regestering_a_new_function_to_the_same_url_and_method)
    {
        //given we have two different functions registered to the same Url/Method combination
        EndpointRegistry registry;
        Endpoint originalEndpoint = [](HttpMessage message)
        {
            return HttpMessage{200,{}, "Original endpoint called."};
        };
        Endpoint overridingEndpoint = [](HttpMessage message)
        {
            return HttpMessage{200,{}, "Overriding endpoint called."};
        };
        registry.registerEndpoint("/hello", HttpMessage::Method::GET, originalEndpoint);
        registry.registerEndpoint("/hello", HttpMessage::Method::GET, overridingEndpoint);

        //when we send a request to that url using the appropriate method
        auto request = HttpMessage{HttpMessage::Method::GET,"/hello",{},"apples"};
        auto actual = registry.submitMessage(request);

        //then we'll get back a response from the most recently registered handler.
        HttpMessage expected = {200,{}, "Overriding endpoint called."};
        EXPECT_EQ(expected, actual);
    }

    TEST(registerEndpoint, will_register_the_same_function_to_multiple_endpoints)
    {
        //given we have two different functions that are registered to different methods
        EndpointRegistry registry;
        const Endpoint methodFunction =[](HttpMessage message)
        {
            return HttpMessage{200,{}, message.getHttpMethodAsString() + " endpoint called."};
        };
        registry.registerEndpoint("/hello", HttpMessage::Method::GET, methodFunction);
        registry.registerEndpoint("/hello", HttpMessage::Method::PUT, methodFunction);

        //when we send a request to the url using the proper methods
        const HttpMessage actual[2] =
        {
            registry.submitMessage(HttpMessage{HttpMessage::Method::GET,"/hello",{},"apples"}),
            registry.submitMessage(HttpMessage{HttpMessage::Method::PUT,"/hello",{},"apples"})
        };

        //then we get back the expected responses
        const HttpMessage expected[2] =
        {
            {200,{},"GET endpoint called."},
            {200, {}, "PUT endpoint called."}
        };
        EXPECT_EQ(actual[0], expected[0]);
        EXPECT_EQ(actual[1], expected[1]);
    }
}