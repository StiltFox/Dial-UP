add_executable(HttpMessageTests
        Parsing.c++
        printAsResponse.c++
        printAsRequest.c++
)

target_link_libraries(HttpMessageTests
        gtest
        gtest_main
        nlohmann_json
        PrintHelper
        HttpMessage
)