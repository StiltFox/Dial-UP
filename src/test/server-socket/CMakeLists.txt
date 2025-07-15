add_executable(SocketTests
        SocketTest.c++
)

target_link_libraries(SocketTests
        gtest
        gtest_main
        nlohmann_json
        PrintHelper
        HttpMessage
        ServerSocket
)