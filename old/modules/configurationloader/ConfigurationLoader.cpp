#include "ConfigurationLoader.hpp"
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;
using namespace StiltFox::Web::Framework;
using namespace StiltFox::UniversalLibrary;

const unordered_map<string,Logger::Level> levelMap = {{"DEBUG", Logger::DEBUG},{"INFO", Logger::INFO},{"WARN", Logger::WARN}, {"ERROR", Logger::ERROR}};

template <typename t>
inline t getJsonDefaultValue(json input, string property, t dflt)
{
    t output = dflt;
    try
    {
        output = input.at(property).get<t>();
    }
    catch(...)
    {
        //do nothing and return default
    }
    return output;
}

inline Logger::Level getLogLevel(string level)
{
    Logger::Level output = Logger::ERROR;

    if (levelMap.contains(level)) output = levelMap.at(level);

    return output;
}

ConfigurationLoader::ConfigurationLoader(string path)
{
    File configFile(path);

    json configJson = json::parse(configFile.read(), nullptr, false);
    port = getJsonDefaultValue(configJson, "portNumber", 8080);
    string logFile = getJsonDefaultValue<string>(configJson, "logFile", "");
    Logger::Level logLevel = getLogLevel(getJsonDefaultValue<string>(configJson, "logLevel", "ERROR"));
    logger = logFile == "" ? new Logger(logLevel) : new FileLogger(logFile, true, logLevel);

    if (configJson.is_discarded() && path != "") logger->error("Could not read config file at: " + path + "\n");
}

ConfigurationLoader::~ConfigurationLoader()
{
    if (logger != nullptr) delete logger;
}