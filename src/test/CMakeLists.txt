if(NOT SFSkipTesting)
    find_package(GTest REQUIRED)
    find_package(nlohmann_json REQUIRED)

    add_subdirectory(print_helper)

    add_subdirectory(http-message)
    add_subdirectory(server-socket)
    add_subdirectory(endpoint-registry)
    add_subdirectory(url)
    add_subdirectory(port-authority)
endif ()