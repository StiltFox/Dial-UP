add_library(ThreadPool ThreadPool.c++)

set_target_properties(ThreadPool PROPERTIES PUBLIC_HEADER "src/main/thread-pool/ThreadPool.h++")
target_include_directories(ThreadPool PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")