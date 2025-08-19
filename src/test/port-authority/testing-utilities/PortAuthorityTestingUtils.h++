/*******************************************************
* Created by cryos on 8/18/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifndef Stilt_Fox_fe61f031c41f45ceb1b571ed77b0cd7d
#define Stilt_Fox_fe61f031c41f45ceb1b571ed77b0cd7d
#include "PortAuthority.h++"

namespace StiltFox::DialUp::Tests::PortAuthorityTests
{
    typedef std::unordered_map<PortAuthority::LogSevarity,std::vector<std::string>> LogMap;

    std::shared_ptr<LogMap> setLoggerOnPortAuthority(PortAuthority& authority,std::mutex& loggerMutex);
    bool checkLogMapForValue(const PortAuthority::LogSevarity& logLevel,const LogMap& logMap,const std::string& value);
    bool checkForErrors(std::mutex& loggerMutex,const LogMap& logMap);
    bool checkForServerSuccessfulBootUp(std::mutex& loggerMutex, const LogMap& logMap);
    void waitForApplicationBootup(std::mutex& loggerMutex,const LogMap& logMap);
    void waitForApplicationBootupThenPowerDown(std::mutex& loggerMutex,PortAuthority& application,const LogMap& logMap);
    size_t curlCallBack(char* ptr,size_t size,size_t nmemb,void* userdata);
    HttpMessage sendHttpRequest(const HttpMessage& message);
}
#endif