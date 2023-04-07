#include "ConfigurationLoader.hpp"
#include <Stilt_Fox/UniversalLibrary/File.hpp>
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;
using namespace StiltFox::RestApi_Framework;
using namespace StiltFox::UniversalLibrary;

ConfigurationLoader::ConfigurationLoader(string path)
{
    logFile = File("asdf");
    File configFile(path);
    json configJson = json::parse(configFile.read(), nullptr, false);

    if (configJson.contains("logOutputFile") && configJson.at("logOutputFile").is_string())
    {
        logFile = File(configJson["logOutputFile"].get<string>());
    }
}