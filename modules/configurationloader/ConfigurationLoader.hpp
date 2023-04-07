#ifndef StiltFox_RestAPI_Framework_ConfigurationLoader
#define StiltFox_RestAPI_Framework_ConfigurationLoader
#include <string>
#include <Stilt_Fox/UniversalLibrary/File.hpp>

namespace StiltFox
{
    namespace RestApi_Framework
    {
        class ConfigurationLoader
        {
            int listeningPort;
            UniversalLibrary::File logFile;

            public:
            ConfigurationLoader(std::string fileLocation);
        };
    }
}

#endif