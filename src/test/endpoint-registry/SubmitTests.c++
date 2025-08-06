/*******************************************************
* Created by cryos on 7/16/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include "EndpointRegistry.h++"

using namespace std;

namespace StiltFox::DialUp::Tests::EndpointRegistryTests::Submit
{
    Endpoint registeredEndpoint = [](HttpMessage message)
    {
        return HttpMessage{200,{},"Message received:\r\n\r\n" + message.printAsRequest()};
    };

    EndpointRegistry setupEndpointRegistry()
    {
        EndpointRegistry registry;
        registry.registerEndpoint("/multi-method-endpoint",HttpMessage::Method::GET,registeredEndpoint);
        registry.registerEndpoint("/multi-method-endpoint",HttpMessage::Method::PUT,registeredEndpoint);
        registry.registerEndpoint("/single-method-endpoint",HttpMessage::Method::PATCH,registeredEndpoint);
        return registry;
    }

    TEST(submitMessage, will_return_a_404_message_if_the_url_is_not_registered)
    {
        //given we have a registry
        EndpointRegistry registry = setupEndpointRegistry();

        //when we submit a message for an unregistered url
        const auto actual = registry.submitMessage(HttpMessage{HttpMessage::Method::GET,"/non-existing-endpoint"});

        //then we get back a 404 message
        const HttpMessage expected = {404,{},""};
        EXPECT_EQ(actual,expected);
    }

    TEST(submitMessage, will_return_method_not_allowed_if_a_url_exists_but_does_not_accept_the_requested_method)
    {
        //given we have a registry
        EndpointRegistry registry = setupEndpointRegistry();

        //when we submit a message for an unregistered method on a registered url
        const auto actual = registry.submitMessage({HttpMessage::Method::TRACE, "/multi-method-endpoint"});

        //then we get back a 405 message
        const HttpMessage expected = {405,{},""};
        EXPECT_EQ(actual,expected);
    }

    TEST(submitMessage, will_return_the_result_of_the_regestered_function_when_a_valid_url_and_method_are_given)
    {
        //given we have a registry
        EndpointRegistry registry = setupEndpointRegistry();

        //when we submit a message for a registered url and method
        const auto actual = registry.submitMessage({HttpMessage::Method::GET,"/multi-method-endpoint"});

        //then we get back the response from the actual endpoint
        const HttpMessage expected = {200,{},"Message received:\r\n\r\nGET /multi-method-endpoint HTTP/1.1\r\n\r\n"};
        EXPECT_EQ(actual,expected);
    }

    TEST(submitMessage, will_send_a_message_to_a_wildcard_if_no_other_pathing_options_are_available)
    {
        //given we have a url registered with a wildcard
        EndpointRegistry registry;
        const Endpoint methodFunction =[](HttpMessage message)
        {
            return HttpMessage{200,{}, message.requestUri.toUrl() + " endpoint reached"};
        };
        registry.registerEndpoint("/record-id/*/stats", HttpMessage::Method::GET, methodFunction);

        //when we send a request to the url with the star filled in with an actual value
        const auto actual = registry.submitMessage(HttpMessage{HttpMessage::GET,"/record-id/123/stats",{},""});

        //then we get back the expected response
        const HttpMessage expected = {200,{},"/record-id/123/stats endpoint reached"};
        EXPECT_EQ(actual, expected);
    }

    TEST(submitMessage, will_prioritize_defined_paths_over_wildcard_paths)
    {
        //given we have a url registered with a wildcard, and one in the same path without a wildcard
        EndpointRegistry registry;
        const Endpoint methodFunction =[](HttpMessage message)
        {
            return HttpMessage{200,{}, message.requestUri.toUrl() + " endpoint reached"};
        };
        registry.registerEndpoint("/record-id/*/stats",HttpMessage::Method::GET,registeredEndpoint);
        registry.registerEndpoint("/record-id/pickle/stats",HttpMessage::Method::GET,methodFunction);

        //when we send a request to the defined url, and an undefiend one
        const HttpMessage actual[2] =
        {
            registry.submitMessage(HttpMessage{HttpMessage::Method::GET,"/record-id/pickle/stats",{},""}),
            registry.submitMessage(HttpMessage{HttpMessage::Method::GET,"/record-id/123/stats",{},""})
        };

        //then we get back the expected responses
        const HttpMessage expected[2] =
        {
            {200,{},"/record-id/pickle/stats endpoint reached"},
            {200,{},"Message received:\r\n\r\nGET /record-id/123/stats HTTP/1.1\r\n\r\n"}
        };
    }
}