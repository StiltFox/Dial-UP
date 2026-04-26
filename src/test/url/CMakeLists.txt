add_executable(UrlTests
        ToUrlTests.c++
        ToUrlWithoutParameters.c++
        parseTests.c++
)

target_link_libraries(UrlTests
        gtest
        gtest_main
        nlohmann_json
        PrintHelper
        URL
)

add_test(NAME Url COMMAND UrlTests)

