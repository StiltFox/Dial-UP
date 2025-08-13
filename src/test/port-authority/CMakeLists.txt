find_package(CURL REQUIRED)

add_executable(PortAuthorityTests
        StartApplicationTests.c++
)

target_link_libraries(PortAuthorityTests
        gtest
        gtest_main
        nlohmann_json
        CURL::libcurl
        PrintHelper
        HttpMessage
        ServerSocket
        ClientConnection
        PortAuthority
)