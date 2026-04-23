/*******************************************************
* Created by Cryos on 4/20/26.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include "PortAuthority.h++"
#include "PrintHelper.h++"
#include "HttpClient.h++"
#include "PortAuthorityTestingUtils.h++"

using namespace std;
using namespace StiltFox::DialUp;
using namespace StiltFox::DialUp::Tests::PortAuthorityTests;

HttpMessage processRequest(const HttpMessage& input)
{
  return {200, {}, "processed request: \n\n" + input.body};
}

TEST(sendWebRequest, will_send_a_request_to_the_desired_external_resource)
{
  //given we have an HttpClient and an external website to hit
  PortAuthority authority(2000,2010,10);
  mutex loggerMutex;
  auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);
  thread serverThread([&authority](){authority.startApplication();});
  waitForApplicationBootup(loggerMutex, *logMap);
  authority.registry.registerEndpoint("/", HttpMessage::GET, processRequest);
  HttpClient* client = HttpClient::getInstance();

  //when we try to send a request
  HttpMessage request(HttpMessage::GET,"http://localhost:2000/",{},"test Request");
  const auto actual = client->sendWebRequest(request);

  //then it is recieved and we get the response back
  HttpMessage expected{200, {}, "processed request: \n\n" + request.body};
  EXPECT_EQ(actual, expected);
  authority.stopApplication();
  serverThread.join();
}
