#include "ConfigurationLoader.hpp"
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;
using namespace StiltFox::Web::Framework;
using namespace StiltFox::UniversalLibrary;

unordered_map<string,Logger::Level> levelMap = {{"DEBUG", Logger::DEBUG},{"INFO", Logger::INFO},{"WARN", Logger::WARN}, {"ERROR", Logger::ERROR}};

ConfigurationLoader::ConfigurationLoader(string path)
{
    logger = nullptr;
    File configFile(path);

    if (configFile.exists())
    {
        json configJson = json::parse(configFile.read(), nullptr, false);
        port = configJson.contains("port") && configJson["port"].is_number() ? configJson["port"].get<int>() : 8080;
        if (configJson.contains("logFile") && configJson["logFile"].is_string())
        {
            logger = new FileLogger(configJson["logFile"].get<string>(), true, Logger::INFO);
        }
        else
        {
            logger = new Logger(Logger::DEBUG);
        }   
    }
    else
    {
        if (logger == nullptr) logger = new Logger(Logger::DEBUG);
        logger->error("could not read config file at: " + path + "\n");
        port = 8080;
    }
}

ConfigurationLoader::~ConfigurationLoader()
{
    if (logger != nullptr) delete logger;
}