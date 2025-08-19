add_library(ServerSocket ServerSocket.c++)

add_subdirectory(client-connection)

set_target_properties(ServerSocket PROPERTIES PUBLIC_HEADER "src/main/socket/Socket.h++")
target_include_directories(ServerSocket PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")