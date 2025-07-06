if(NOT SFSkipTests)
    find_package(GTest REQUIRED)
    find_package(nlohmann_json REQUIRED)

    add_subdirectory(print_helper)

    add_subdirectory(http-message)
endif ()