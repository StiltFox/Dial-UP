add_library(PortAuthority PortAuthority.c++)

target_link_libraries(PortAuthority
        HttpMessage
        EndpointRegistry
        ClientConnection
        ServerSocket
)

set_target_properties(PortAuthority PROPERTIES PUBLIC_HEADER "src/main/port-authority/PortAuthority.h++")
target_include_directories(PortAuthority PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")