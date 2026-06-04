/*******************************************************
* Created by Cryos on 8/21/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "PortAuthority.h++"
#include "PrintHelper.h++"
#include "HttpClient.h++"
#include "PortAuthorityTestingUtils.h++"

using namespace std;
using namespace nlohmann;

namespace StiltFox::DialUp::Tests::PortAuthorityTests::EndpointIntegrationTests
{
    TEST(Integration, endpoints_will_return_a_response_to_the_caller)
    {
        //given we have a running port authority running in a thread
        PortAuthority authority(2000,2010,10);
        mutex loggerMutex;
        auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);
        thread serverThread([&authority](){authority.startApplication();});
        waitForApplicationBootup(loggerMutex, *logMap);

        //and we register an endpoint
        Endpoint endpoint([](HttpMessage msg){return HttpMessage{200};});
        authority.registry.registerEndpoint("/home",HttpMessage::Method::GET,endpoint);

        //when we send an http request
        const auto client = HttpClient::getInstance();
        const auto actual = client->sendWebRequest({HttpMessage::Method::GET, "http://localhost:2000/home"});

        //then an expected response is sent
        json logMapJson = *logMap;
        EXPECT_EQ(actual,HttpMessage{200}) << logMapJson.dump(4);
        authority.stopApplication();
        serverThread.join();
    }

    TEST(Integration, endpoint_will_return_an_error_when_client_takes_too_long_to_send_data)
    {
        //given we have a running port authority in another thread
        PortAuthority authority (2000, 2010, 10, 1); //make it super impatient
        mutex loggerMutex;
        auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);
        thread serverThread([&authority](){authority.startApplication();});
        waitForApplicationBootup(loggerMutex, *logMap);

        //and we have an endpoint
        Endpoint endpoint([](HttpMessage msg){return HttpMessage{200};});
        authority.registry.registerEndpoint("/", HttpMessage::Method::GET, endpoint);

        //when we send an http request that should time out
        HttpMessage longMessage = {HttpMessage::GET, "http://localhost:2000",{}, ""};
        for (int x=0; x<200000; x++) longMessage.body += 0x42;
        HttpClient* client = HttpClient::getInstance();
        const HttpMessage response = client->sendWebRequest(longMessage);

        //then we get back a timeout
        HttpMessage expectedResponse(408);
        EXPECT_EQ(response, expectedResponse);

        //clean up
        authority.stopApplication();
        serverThread.join();
    }

    TEST(Integration, endpoint_will_return_an_error_when_the_client_sends_too_much_data)
    {
        //given we have a running port authority in another thread
        PortAuthority authority(2000, 2010, 10, 10000, 1); //set really low data limit
        mutex loggerMutex;
        auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);
        thread serverThread([&authority]{authority.startApplication();});
        waitForApplicationBootup(loggerMutex, *logMap);

        //and we register an endpoint
        Endpoint endpoint([](HttpMessage msg){return HttpMessage{200};});
        authority.registry.registerEndpoint("/", HttpMessage::GET, endpoint);

        //when we send an http request that has too much data
        HttpMessage longMessage{HttpMessage::GET, "http://localhost:2000",{}};
        for (int x=0; x<20; x++) longMessage.body += 0x42;
        HttpClient* client = HttpClient::getInstance();
        const HttpMessage response = client->sendWebRequest(longMessage);

        //then we get back a bad request
        HttpMessage expectedResponse{413};
        EXPECT_EQ(response, expectedResponse);

        //clean up
        authority.stopApplication();
        serverThread.join();
    }
}
