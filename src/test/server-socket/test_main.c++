#include <gtest/gtest.h>
#include "HttpClient.h++"

StiltFox::DialUp::HttpClient* client = nullptr;

int main(int argc, char **argv)
{
  client = StiltFox::DialUp::HttpClient::getInstance();
  testing::InitGoogleTest(&argc, argv);
  const auto output = RUN_ALL_TESTS();

  if (client != nullptr) delete client;

  return output;
}
