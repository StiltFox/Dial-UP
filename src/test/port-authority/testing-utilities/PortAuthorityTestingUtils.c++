/*******************************************************
* Created by Cryos on 8/18/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <curl/curl.h>
#include "PortAuthorityTestingUtils.h++"
#include <algorithm>
#include "ServerSocket.h++"

namespace StiltFox::DialUp::Tests::PortAuthorityTests
{
    std::shared_ptr<LogMap> setLoggerOnPortAuthority(PortAuthority& authority,std::mutex& loggerMutex)
    {
        auto logger = std::make_shared<LogMap>();

        authority.logger = [logger, &loggerMutex](PortAuthority::LogSevarity logLevel, std::string message)
        {
            std::lock_guard lock(loggerMutex);
            (*logger)[logLevel].push_back(message);
        };

        return logger;
    }

    bool checkLogMapForValue(const PortAuthority::LogSevarity& logLevel,const LogMap& logMap,const std::string& value)
    {
        const auto logLevelValues = logMap.contains(logLevel) ? logMap.at(logLevel) : std::vector<std::string>();
        return std::ranges::find(logLevelValues, value) != logLevelValues.end();
    }

    bool checkForErrors(std::mutex& loggerMutex,const LogMap& logMap)
    {
        bool output = false;

        std::lock_guard guard(loggerMutex);
        if (logMap.contains(PortAuthority::LogSevarity::ERROR) && !logMap.at(PortAuthority::LogSevarity::ERROR).empty())
            output = true;

        return output;
    }

    bool checkForServerSuccessfulBootUp(std::mutex& loggerMutex, const LogMap& logMap)
    {
        std::lock_guard guard(loggerMutex);
        return checkLogMapForValue(PortAuthority::LogSevarity::INFO, logMap, "Server started successfully");
    }

    void waitForApplicationBootup(std::mutex& loggerMutex, const LogMap& logMap)
    {
        auto startTime = std::chrono::system_clock::now();
        while(std::chrono::system_clock::now() - startTime < std::chrono::seconds(5) &&
            !checkForErrors(loggerMutex, logMap) && !checkForServerSuccessfulBootUp(loggerMutex, logMap));
    }

    void waitForApplicationBootupThenPowerDown(std::mutex& loggerMutex,PortAuthority& application,const LogMap& logMap)
    {
        waitForApplicationBootup(loggerMutex, logMap);
        application.stopApplication();
    }
}
