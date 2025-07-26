add_executable(UrlTests
        ToUrlTests.c++
        ToUrlWithoutParameters.c++
)

target_link_libraries(UrlTests
        gtest
        gtest_main
        nlohmann_json
        PrintHelper
        URL
)