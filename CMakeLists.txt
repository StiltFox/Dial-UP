cmake_minimum_required(VERSION 3.0.0)
project(StiltFox-Web-Framework VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest)
find_package(UniversalLibrary REQUIRED)

include(GoogleTest)
enable_testing()

include_directories(modules/controllermanager modules/configurationloader)

add_subdirectory(modules)
add_library(StiltFox-Web-Framework StiltFox-Web-Framework.cpp)
target_link_libraries(StiltFox-Web-Framework ConfigurationLoader)

add_executable(BehaviorTests StiltFox-Web-FrameworkTest.cpp)
target_link_libraries(BehaviorTests StiltFox-Web-Framework ConfigurationLoader GTest::Main UniversalLibrary::File UniversalLibrary::CoutCapture UniversalLibrary::TempFile)
gtest_discover_tests(BehaviorTests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
