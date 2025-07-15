cmake_minimum_required(VERSION 3.5.0)
project(Dial-Up VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_subdirectory(src)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/DialUpConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DialUp
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/DialUpConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/DialUpVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/StorageShed
)

install(
        TARGETS EndpointRegistry HttpMessage PortAuthority ServerSocket ClientConnection
        EXPORT DialUp
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/Stilt_Fox/DialUp
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/Stilt_Fox/DialUp
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/Stilt_Fox/DialUp
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Stilt_Fox/DialUp
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Stilt_Fox/DialUp
)

install(EXPORT DialUp
        FILE DialUpTargets.cmake
        NAMESPACE StiltFox::DialUp::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DialUp
)

# generate the version file for the config file
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/DialUpConfigVersion.cmake"
        VERSION "${version}"
        COMPATIBILITY AnyNewerVersion
)