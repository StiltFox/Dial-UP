add_executable(HttpMessageTests
        Parsing.c++
)

target_link_libraries(HttpMessageTests
        gtest
        gtest_main
        nlohmann_json
        PrintHelper
        HttpMessage
)