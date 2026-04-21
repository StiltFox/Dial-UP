add_executable(HttpClientTests
  getInstance.c++
)

target_link_libraries(HttpClientTests
  gtest
  gtest_main
  nlohmann_json
  HttpClients
)

