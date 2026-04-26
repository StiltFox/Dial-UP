/*******************************************************
* Created by Cryos on 4/20/26.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include "HttpClient.h++"

/***********************************************************************************************************************
 * IMPORTANT: As much as I may have wanted to... testing the single initialization of curl does not seem to be possible
 * right now with current tools or exercizing some type of sorcery. That being said we will just trust that the code 
 * only initializes curl one time.
 *
 * GTest: says there's no way to mock free functions or C style functions. As much as it may be tempting to write a
 * wrapper or some other nonsense just for testing purposes, the test isnt the product. The library is the product. No
 * abstraction is free and I will not sacrifice production code performance or integrity for a stupid test.
 **********************************************************************************************************************/

using namespace StiltFox::DialUp;

TEST(getInstance, will_always_return_the_same_instance_of_http_client)
{
  //given we have initialized HttpClient
  const auto client = HttpClient::getInstance();

  //when we try to retrieve anotehr client
  const auto client2 = HttpClient::getInstance();

  //then both returned clients will be the same
  EXPECT_EQ(client, client2); //it's ok if we have no print helper here. the mem addresses should match.
  if (client != nullptr) delete client;
}

//please note, that the pointer is deleted here because these tests should be running single threaded. Idealy this
//should happen at the end of all tests for this class to save spin-up time... but meh. They're just tests and there are
//not that many.
