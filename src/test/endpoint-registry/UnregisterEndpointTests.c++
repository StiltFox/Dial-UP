/*******************************************************
* Created by cryos on 7/16/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include "PrintHelper.h++"
#include "EndpointRegistry.h++"

using namespace std;

namespace StiltFox::DialUp::Test::EndpointRegistryTests::UnregisterEndpoint
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

    TEST(unregisterEndpoint, will_do_nothing_if_you_unregister_a_non_existing_url)
    {
        //given we have a registry
        EndpointRegistry registry = setupEndpointRegistry();

        //when we remove a url that is not registered
        registry.unregisterEndpoint("/non-existing-endpoint", HttpMessage::Method::GET);

        //then nothing changes
        EndpointRegistry expected = setupEndpointRegistry();
        EXPECT_EQ(registry.getRegisteredEndpoints(), expected.getRegisteredEndpoints());
    }

    TEST(unregisterEndpoint, will_remove_the_requested_endpoint_from_the_registry)
    {
        //given we have a registry
        EndpointRegistry registry = setupEndpointRegistry();

        //when we remove an existing endpoint
        registry.unregisterEndpoint("/multi-method-endpoint", HttpMessage::Method::GET);

        //then we have unregistered the endpoint
        const unordered_map<string,unordered_set<HttpMessage::Method>> expected =
        {
            {"/multi-method-endpoint", {HttpMessage::Method::PUT}},
            {"/single-method-endpoint", {HttpMessage::Method::PATCH}}
        };
        EXPECT_EQ(registry.getRegisteredEndpoints(), expected);
    }

    TEST(unregisterEndpoint, will_remove_the_url_from_the_map_if_it_no_longer_serves_any_methods)
    {
        //given we have a registry
        EndpointRegistry registry = setupEndpointRegistry();

        //when we remove an existing endpoint that has only one function registered to it
        registry.unregisterEndpoint("/single-method-endpoint",HttpMessage::Method::PATCH);

        //then we have removed the url entirely from the registry
        const unordered_map<string,unordered_set<HttpMessage::Method>> expected =
        {
            {"/multi-method-endpoint", {HttpMessage::Method::PUT,HttpMessage::Method::GET}}
        };
        EXPECT_EQ(registry.getRegisteredEndpoints(), expected);
    }
}