/*******************************************************
* Created by Cryos on 7/10/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <codecvt>
#include <thread>
#include <gtest/gtest.h>
#include <curl/curl.h>
#include "PrintHelper.h++"
#include "ServerSocket.h++"

using namespace std;

namespace StiltFox::DialUp
{
    TEST(openPort, will_return_true_if_the_port_is_available_and_bound)
    {
        //given we have a port that's not used.
        ServerSocket openPort(4200);

        //when we open the port for listening
        const bool actual = openPort.openPort();

        //then we get back that the port is opened
        EXPECT_TRUE(actual);
    }

    TEST(openPort, will_return_false_if_the_port_is_already_taken)
    {
        //given we have a port that's already open
        ServerSocket openPort(4200);
        ServerSocket usedPort(4200);
        openPort.openPort();

        //when we open the second port
        const bool actual = usedPort.openPort();

        //then we get back that the port could not be opened
        EXPECT_FALSE(actual);
    }

    TEST(closePort, will_make_the_port_available_for_use_again)
    {
        //given we have a port that's already open
        ServerSocket openPort(4200);
        ServerSocket usedPort(4200);
        openPort.openPort();

        //when close the port then open the second port
        openPort.closePort();
        const bool actual = usedPort.openPort();

        //then we get back that the port could not be opened
        EXPECT_TRUE(actual);
    }

    TEST(deconstructor, going_out_of_scope_will_close_the_port)
    {
        //given we open a socket within a contained scope
        {
            ServerSocket openPort(4200);
            openPort.openPort();
        }

        //when we open a new socket on the same port number
        ServerSocket usedPort(4200);
        auto actual = usedPort.openPort();

        //then the port can be opened because the previous one was closed
        EXPECT_TRUE(actual);
    }

    TEST(Connection, receiveData_will_get_the_information_sent_to_it_by_the_client)
    {
        //given we have a port that we're listing to
        auto openPort = make_shared<ServerSocket>(4200);
        openPort->openPort();
        HttpMessage expectedData(HttpMessage::POST, "/asdf", {
            {"Accept",{"*/*"}},
            {"Content-Length",{"30"}},
            {"Content-Type",{"application/x-www-form-urlencoded"}},
            {"Host", {"localhost:4200"}}
        }, "漬物はいいですよね。");

        //when we open a connection
        auto actual = make_shared<HttpMessage>(HttpMessage::Method::ERROR);

        thread connectionListenerThread([openPort, &actual](){
            ClientConnection clientConnection(openPort->getHandle(), openPort->getAddress());
            const auto responseData = clientConnection.receiveData();
            actual = make_shared<HttpMessage>(responseData.data);
        });

        //and we send data to the port
        CURL* curl;

        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:4200/asdf");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, expectedData.body.c_str());
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        connectionListenerThread.join();

        //then the data is received
        EXPECT_EQ(*actual, expectedData);
    }
}