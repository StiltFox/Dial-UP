#include "ConfigurationLoader.hpp"
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;
using namespace StiltFox::Web::Framework;
using namespace StiltFox::UniversalLibrary;

streambuf* defaultBuff;

ConfigurationLoader::ConfigurationLoader(string path)
{
    logger = nullptr;
    // logStream = nullptr;
    // defaultBuff = cout.rdbuf();
    // File configFile(path);

    // if (configFile.exists())
    // {
    //     json configJson = json::parse(configFile.read(), nullptr, false);
    //     port = configJson.contains("port") && configJson["port"].is_number() ? configJson["port"].get<int>() : 8080;

    //     if (configJson.contains("logFile") && configJson["logFile"].is_string())
    //     {
    //         File log = configJson["logFile"].get<string>();
    //         if (log.canWrite()) 
    //         {
    //             log.touch();
    //             logStream = new ofstream(log.getPath());
    //             cout.rdbuf(logStream->rdbuf());
    //         }
    //     }
    // }
    // else
    // {
    //     cout << endl << "could not read config file at: " << path << endl;
    //     port = 8080;
    // }
}

ConfigurationLoader::~ConfigurationLoader()
{
    if (logger != nullptr) delete logger;
}