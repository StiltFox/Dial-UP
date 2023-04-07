cmake_minimum_required(VERSION 3.0.0)
project(StiltFox-RestApi-Framework VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest)
find_package(UniversalLibrary REQUIRED)

include(GoogleTest)
enable_testing()

include_directories(modules/controllermanager modules/configurationloader)

add_subdirectory(modules)
add_executable(StiltFox-RestApi-Framework StiltFox-RestApi-Framework.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
