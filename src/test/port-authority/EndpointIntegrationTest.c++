/*******************************************************
* Created by Cryos on 8/21/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "PrintHelper.h++"
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
        auto actual = sendHttpRequest({HttpMessage::Method::GET, "http://localhost:2000/home"});

        //then an expected response is sent
        json logMapJson = *logMap;
        EXPECT_EQ(actual,HttpMessage{200}) << logMapJson.dump(4);
        authority.stopApplication();
        serverThread.join();
    }
}