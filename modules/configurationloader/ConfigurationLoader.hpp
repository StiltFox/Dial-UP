#ifndef StiltFox_Web_Framework_ConfigurationLoader
#define StiltFox_Web_Framework_ConfigurationLoader
#include <string>
#include <Stilt_Fox/UniversalLibrary/FileLogger.hpp>

namespace StiltFox
{
    namespace Web
    {
        namespace Framework
        {
            struct ConfigurationLoader
            {
                int port;
                UniversalLibrary::Logger* logger;

                ConfigurationLoader(std::string fileLocation);
                ~ConfigurationLoader();
            };
        }
    }
}

#endif