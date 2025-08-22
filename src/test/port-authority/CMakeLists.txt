find_package(CURL REQUIRED)
add_subdirectory(testing-utilities)

add_executable(PortAuthorityTests
        StartApplicationTests.c++
        KillListenerTests.c++
        EndpointIntegrationTest.c++
)

target_link_libraries(PortAuthorityTests
        gtest
        gtest_main
        nlohmann_json
        CURL::libcurl
        PortAuthorityTestingUtils
        PrintHelper
        HttpMessage
        ServerSocket
        ClientConnection
        PortAuthority
)