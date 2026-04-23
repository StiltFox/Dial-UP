add_executable(HttpClientTests
  getInstance.c++
  sendWebRequest.c++
)

target_link_libraries(HttpClientTests
  gtest
  gtest_main
  nlohmann_json
  HttpClient
  PortAuthority
  PortAuthorityTestingUtils
  PrintHelper
)

add_test(NAME Http-Client COMMAND HttpClientTests)

