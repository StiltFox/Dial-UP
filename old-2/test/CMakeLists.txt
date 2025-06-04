add_executable(Tests HttpMessageTest.c++ SocketTest.c++)
target_link_libraries(Tests
        GTest::gtest_main
        HttpMessage
        Socket
)
gtest_discover_tests(Tests)