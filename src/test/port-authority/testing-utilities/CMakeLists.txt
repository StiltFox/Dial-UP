add_library(PortAuthorityTestingUtils PortAuthorityTestingUtils.c++)

target_link_libraries(PortAuthorityTestingUtils
        HttpMessage
        nlohmann_json
        URL
        PortAuthority
        CURL::libcurl
)

target_include_directories(PortAuthorityTestingUtils PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")