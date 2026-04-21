add_library(HttpClient HttpClient.c++)

target_link_libraries(HttpClient
  HttpMessage
)

set_target_properties(HttpClient PROPERTIES PUBLIC_HEADER "src/main/http-client/HttpClient.h++")
target_include_directories(HttpClient PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>" 
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
