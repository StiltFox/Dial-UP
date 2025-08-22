find_package(CURL REQUIRED)

add_executable(SocketTests
        ServerSocketTest.c++
)

target_link_libraries(SocketTests
        gtest
        gtest_main
        nlohmann_json
        CURL::libcurl
        PrintHelper
        HttpMessage
        ServerSocket
        PortAuthorityTestingUtils
        ClientConnection
)