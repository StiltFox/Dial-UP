add_library(Socket Socket.c++)

target_link_libraries(Socket
        HttpMessage
)

set_target_properties(Socket PROPERTIES PUBLIC_HEADER "src/main/socket/Socket.h++")
target_include_directories(Socket PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")