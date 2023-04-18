#ifndef StiltFox_Web_Framework
#define StiltFox_Web_Framework
#include "ControllerManager.hpp"
#include "ConfigurationLoader.hpp"

namespace StiltFox
{
    namespace Web
    {
        namespace Framework
        {
            struct WebApp
            {
                ConfigurationLoader* configuration;
                //ControllerManager* routing;
                WebApp(std::string configurationLocation = "/etc/StiltFox/Web/Conf.json");
                ~WebApp();
                void start();
            };
        }
    }
}

#endif