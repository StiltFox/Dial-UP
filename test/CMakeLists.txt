add_executable(Tests HttpMessageTest.c++)
target_link_libraries(Tests
        GTest::gtest_main
        HttpMessage
)
gtest_discover_tests(Tests)