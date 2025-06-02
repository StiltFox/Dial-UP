cmake_minimum_required(VERSION 3.5.0)
project(Dial-Up VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(modules)

if(NOT SFSkipTests)
    find_package(GTest)
    include(GoogleTest)
    enable_testing()
    add_subdirectory(test)
endif ()
