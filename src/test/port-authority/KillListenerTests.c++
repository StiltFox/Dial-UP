/*******************************************************
* Created by Cryos on 8/18/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>
#include "PrintHelper.h++"
#include "HttpClient.h++"
#include "PortAuthorityTestingUtils.h++"

using namespace std;
using namespace nlohmann;

namespace StiltFox::DialUp::Tests::PortAuthorityTests::KillListenerTests
{
    TEST(KillSocket, will_return_ok_and_terminate_port_authroity_operations_when_proper_message_is_sent)
    {
        //given we have a running port authority running in a thread
        PortAuthority authority(2000,2010,10);
        bool serverRunning = true;
        mutex loggerMutex;
        auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);
        thread serverThread([&authority, &serverRunning]()
        {
            authority.startApplication();
            serverRunning = false;
        });
        waitForApplicationBootup(loggerMutex, *logMap);

        //when we send the kill command
        const auto client = HttpClient::getInstance();
        const auto response = client->sendWebRequest({HttpMessage::Method::DELETE,"http://localhost:2010", 
                {{"operation",{"kill"}}}});

        //then the application has terminated
        HttpMessage expected = {200,{{"Content-Type",{"text/plain"}}},"shutting down"};
        this_thread::sleep_for(chrono::milliseconds(10));
        EXPECT_FALSE(serverRunning);
        EXPECT_EQ(response,expected);

        //clean up
        if (serverRunning) authority.stopApplication();
        serverThread.join();
    }

    TEST(KillSocket, will_return_bad_request_if_the_sent_kill_message_does_not_meet_kill_message_criteria)
    {
        //given we have a running port authority running in a thread
        PortAuthority authority(2000,2010,10);
        bool serverRunning = true;
        mutex loggerMutex;
        auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);
        thread serverThread([&authority, &serverRunning]()
        {
            authority.startApplication();
            serverRunning = false;
        });
        waitForApplicationBootup(loggerMutex, *logMap);

        //when we send a bad kill command
        const auto client = HttpClient::getInstance();
        const auto response = client->sendWebRequest({HttpMessage::Method::POST,"http://localhost:2010",
                                        {{"operation",{"kill"}}}});

        //then the application has not terminated and we get back a 400
        HttpMessage expected = {400,{{"Content-Type",{"text/plain"}}},"unknown command"};
        EXPECT_TRUE(serverRunning);
        EXPECT_EQ(response,expected);

        //clean up
        if (serverRunning) authority.stopApplication();
        serverThread.join();
    }
}
