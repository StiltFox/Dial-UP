/*******************************************************
* Created by Cryos on 8/18/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <curl/curl.h>
#include "Stilt_Fox/StandMixer/DataProcessor.h++"
#include "PortAuthorityTestingUtils.h++"

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

    size_t curlCallBack(char* ptr, size_t size, size_t nmemb, void* userdata)
    {
        for (int x=0; x<nmemb; x++) ((std::string*)userdata)->append(1,ptr[x]);
        return nmemb;
    }

    HttpMessage sendHttpRequest(const HttpMessage& message)
    {
        HttpMessage output = {-1};
        CURL* curl = curl_easy_init();

        curl_easy_setopt(curl, CURLOPT_URL, message.requestUri.toUrlWithoutParameters().c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.body.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, message.getHttpMethodAsString().c_str());
        curl_slist* headers = nullptr;
        for (const auto& [key,value] : message.headers)
        {
            std::string output = key + ": ";
            for (const auto& trait : value)
            {
                if (!output.ends_with(": ")) output += ",";
                output += trait;
            }
            headers = curl_slist_append(headers, output.c_str());
        }

        if (headers != nullptr) curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&output.body);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        long tempStatusCode;
        curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE, &tempStatusCode);
        output.statusCode = tempStatusCode;

        return output;
    }
}