add_library(ClientConnection ClientConnection.c++)

set_target_properties(ClientConnection PROPERTIES PUBLIC_HEADER
        "src/main/socket/client-connection/clientConnection.h++;src/main/socket/client-connection/Response.h++")
target_include_directories(ClientConnection PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")