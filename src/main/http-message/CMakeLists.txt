add_library(HttpMessage HttpMessage.c++)

target_link_libraries(HttpMessage
        StiltFox::StandMixer::DataProcessor
        URL
)

set_target_properties(HttpMessage PROPERTIES PUBLIC_HEADER "src/main/http-message/HttpMessage.h++")
target_include_directories(HttpMessage PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")