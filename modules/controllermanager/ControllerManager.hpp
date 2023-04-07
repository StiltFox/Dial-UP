#ifndef StiltFox_RestAPI_Framework_ControllerManager
#define StiltFox_RestAPI_Framework_ControllerManager
#include <Stilt_Fox/UniversalLibrary/HttpMessage.hpp>

namespace StiltFox
{
    namespace RestApi_Framework
    {
        class ControllerManager
        {
            std::unordered_map<std::string, std::unordered_map<UniversalLibrary::HttpMessage::Method, std::function<UniversalLibrary::HttpMessage(UniversalLibrary::HttpMessage)>>> registry;

            public:
            UniversalLibrary::HttpMessage dispatchMessage(UniversalLibrary::HttpMessage message);
            void registerController(std::string path, UniversalLibrary::HttpMessage::Method method, std::function<UniversalLibrary::HttpMessage(UniversalLibrary::HttpMessage)>);
        };
    }
}

#endif