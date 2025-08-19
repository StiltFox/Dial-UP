add_library(EndpointRegistry EndpointRegistry.c++)

target_link_libraries(EndpointRegistry
        HttpMessage
)

set_target_properties(EndpointRegistry PROPERTIES PUBLIC_HEADER "src/main/endpoint-registry/EndpointRegistry.h++")
target_include_directories(EndpointRegistry PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")