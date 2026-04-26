find_package(CURL REQUIRED)
add_subdirectory(testing-utilities)

add_executable(PortAuthorityTests
        test_main.c++
        StartApplicationTests.c++
        KillListenerTests.c++
        EndpointIntegrationTest.c++
)

target_link_libraries(PortAuthorityTests
        gtest
        nlohmann_json
        CURL::libcurl
        PortAuthorityTestingUtils
        PrintHelper
        HttpMessage
        ServerSocket
        ClientConnection
        PortAuthority
        HttpClient
)

add_test(NAME Port-Authority COMMAND PortAuthorityTests)

