add_library(URL Url.c++)

target_link_libraries(URL
        StiltFox::StandMixer::DataProcessor
        StiltFox::StandMixer::DataConverter
)

set_target_properties(URL PROPERTIES PUBLIC_HEADER "src/main/url/Url.h++")
target_include_directories(URL PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")