add_library(socket Socket.cpp)
target_link_libraries(socket UniversalLibrary::httpmessage)