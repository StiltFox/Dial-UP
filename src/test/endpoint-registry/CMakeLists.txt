add_executable(EndpointRegistryTests
        RegisterEndpointTests.c++
        UnregisterEndpointTests.c++
        SubmitTests.c++
)

target_link_libraries(EndpointRegistryTests
        gtest
        gtest_main
        nlohmann_json
        PrintHelper
        HttpMessage
        EndpointRegistry
)