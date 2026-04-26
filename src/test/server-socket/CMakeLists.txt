find_package(CURL REQUIRED)

add_executable(SocketTests
        test_main.c++
        ServerSocketTest.c++
)

target_link_libraries(SocketTests
        gtest
        nlohmann_json
        CURL::libcurl
        PrintHelper
        HttpMessage
        ServerSocket
        PortAuthorityTestingUtils
        ClientConnection
        HttpClient
)

add_test(NAME server-socket COMMAND SocketTests)

