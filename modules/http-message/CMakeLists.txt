add_library(HttpMessage HttpMessage.c++)
set_target_properties(HttpMessage PROPERTIES PUBLIC_HEADER "modules/http-message/HttpMessage.c++")
target_include_directories(HttpMessage PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>" "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")