/*******************************************************
* Created by Cryos on 8/12/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "StiltFoxAsciiArt.h++"
#include "PortAuthority.h++"

using namespace std;
using namespace nlohmann;

namespace StiltFox::DialUp::Tests::PortAuthorityTests::StartApplication
{
    typedef unordered_map<PortAuthority::LogSevarity,vector<string>> LogMap;

    inline shared_ptr<LogMap> setLoggerOnPortAuthority(PortAuthority& authority, mutex& loggerMutex)
    {
         auto logger = make_shared<LogMap>();

        authority.logger = [logger, &loggerMutex](PortAuthority::LogSevarity logLevel, string message)
        {
            lock_guard lock(loggerMutex);
            (*logger)[logLevel].push_back(message);
        };

        return logger;
    }

    inline bool checkLogMapForValue(const PortAuthority::LogSevarity& logLevel, const LogMap& logMap,
        const string& value)
    {
        const auto logLevelValues = logMap.contains(logLevel) ? logMap.at(logLevel) : vector<string>();
        return find(logLevelValues.begin(), logLevelValues.end(), value) != logLevelValues.end();
    }

    inline bool checkForErrors(mutex& loggerMutex, const LogMap& logMap)
    {
        bool output = false;

        lock_guard guard(loggerMutex);
        if (logMap.contains(PortAuthority::LogSevarity::ERROR) && !logMap.at(PortAuthority::LogSevarity::ERROR).empty())
            output = true;

        return output;
    }

    inline bool checkForServerSuccessfulBootUp(mutex& loggerMutex, const LogMap& logMap)
    {
        lock_guard guard(loggerMutex);
        return !checkLogMapForValue(PortAuthority::LogSevarity::INFO, logMap, "Server started successfully");
    }

    inline void waitForApplicationBootupThenPowerDown(mutex& loggerMutex, PortAuthority& application,
        const LogMap& logMap)
    {
        auto startTime = chrono::system_clock::now();
        while(chrono::system_clock::now() - startTime < chrono::seconds(5) &&
            checkForErrors(loggerMutex, logMap) && checkForServerSuccessfulBootUp(loggerMutex, logMap));
        application.stopApplication();
    }

    TEST(StartApplication, will_print_a_default_banner_to_standard_console_output_when_called_without_a_banner)
    {
        //given we have a port authority
        PortAuthority authority(110,100,10);
        mutex loggerMutex;
        auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);
        stringstream actual;
        streambuf* coutDefault = cout.rdbuf(actual.rdbuf());

        //when we start the application without a custom banner
        thread serverThread([&authority](){authority.startApplication();});
        waitForApplicationBootupThenPowerDown(loggerMutex,authority,*logMap);
        serverThread.join();

        //then the default banner is printed to the standard console output
        cout.rdbuf(coutDefault);
        EXPECT_TRUE(actual.str().starts_with(DEFAULT_BANNER));
    }

    TEST(StartApplication, will_print_a_custom_banner_to_standard_console_output_when_callled_with_a_banner)
    {
        //given we have a port authority
        PortAuthority authority(110,100,10);
        mutex loggerMutex;
        auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);
        stringstream actual;
        streambuf* coutDefault = cout.rdbuf(actual.rdbuf());

        //when we start the application without a custom banner
        thread serverThread([&authority](){authority.startApplication("this is a test banner");});
        waitForApplicationBootupThenPowerDown(loggerMutex,authority,*logMap);
        serverThread.join();

        //then the banner is printed to the standard console output
        cout.rdbuf(coutDefault);
        EXPECT_TRUE(actual.str().starts_with("this is a test banner"));
    }

    TEST(StartApplication, will_print_an_info_level_log_stating_what_port_the_application_is_running_on)
    {
        //given we have a port authority with a specified port number
        PortAuthority authority(110,100,10);
        mutex loggerMutex;
        auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);

        //when we start the application
        thread serverThread([&authority](){authority.startApplication();});
        waitForApplicationBootupThenPowerDown(loggerMutex,authority,*logMap);
        serverThread.join();

        //then the log contains the port number that the application is listening on.
        const json printableLogMap = *logMap;
        EXPECT_TRUE(checkLogMapForValue(PortAuthority::LogSevarity::INFO, *logMap, "Opening new socket on port 110"))
            << printableLogMap.dump(4);
    }

    TEST(StartApplication, will_print_an_info_level_log_stating_that_the_application_port_has_opened_successfully)
    {
        //given we have a port authority with a specified port number
        PortAuthority authority(2000,100,10);
        mutex loggerMutex;
        auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);

        //when we start the application
        thread serverThread([&authority](){authority.startApplication();});
        waitForApplicationBootupThenPowerDown(loggerMutex,authority,*logMap);
        serverThread.join();

        //then the log contains that the port has been opened successfully
        json printableLogMap = *logMap;
        EXPECT_TRUE(checkLogMapForValue(PortAuthority::LogSevarity::INFO, *logMap, "Port 2000 opened successfully"))
            << printableLogMap.dump(4);
    }

    TEST(StartApplication, will_print_an_error_level_log_stating_that_the_application_port_cannot_be_opened)
    {
        //given we have a port authority with a specified port number
        PortAuthority authority(2000,100,10);
        mutex loggerMutex;
        auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);

        //and the port is already opened
        ServerSocket localPort(2000);
        localPort.openPort();

        //when we start the application
        thread serverThread([&authority](){authority.startApplication();});
        waitForApplicationBootupThenPowerDown(loggerMutex,authority,*logMap);
        serverThread.join();

        //then the log will contain an error stating that the port cant be opened
        json printableLogMap = *logMap;
        EXPECT_TRUE(checkLogMapForValue(PortAuthority::LogSevarity::ERROR, *logMap,
            "Failed to open socket on port 2000")) << printableLogMap.dump(4);
    }

    TEST(StartApplication, will_print_an_info_level_log_stating_that_the_kill_port_is_attempting_to_be_opened)
    {
        //given we have a port authority with a specified kill port number
        PortAuthority authority(2000,2010,10);
        mutex loggerMutex;
        auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);

        //when we start the application
        thread serverThread([&authority](){authority.startApplication();});
        waitForApplicationBootupThenPowerDown(loggerMutex,authority,*logMap);
        serverThread.join();

        //then the log will contain the port number the killswitch is listening too
        json printableLogMap = *logMap;
        EXPECT_TRUE(checkLogMapForValue(PortAuthority::LogSevarity::INFO, *logMap, "Opening new socket on port 2010"))
            << printableLogMap.dump(4);
    }

    TEST(StartApplication, will_print_an_info_level_log_stating_that_the_kill_port_has_opened_sucessfully)
    {
        //given we have a port authority with a specified kill port number
        PortAuthority authority(2000,2010,10);
        mutex loggerMutex;
        auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);

        //when we start the application
        thread serverThread([&authority](){authority.startApplication();});
        waitForApplicationBootupThenPowerDown(loggerMutex,authority,*logMap);
        serverThread.join();

        //then the log will show that the kill port has been successfully opened
        json printableLogMap = *logMap;
        EXPECT_TRUE(checkLogMapForValue(PortAuthority::LogSevarity::INFO, *logMap, "Port 2010 opened successfully"))
            << printableLogMap.dump(4);
    }

    TEST(StartApplication, will_print_an_error_level_log_stating_that_the_kill_port_cannot_be_opened)
    {
        //given we have a port authority with a specified kill port number
        PortAuthority authority(2000,2010,10);
        mutex loggerMutex;
        auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);

        //and the port is already open
        ServerSocket localPort(2010);
        localPort.openPort();

        //when we start the application
        thread serverThread([&authority](){authority.startApplication();});
        waitForApplicationBootupThenPowerDown(loggerMutex,authority,*logMap);
        serverThread.join();

        //then the log will contain an error stating that the kill port could not be opened
        json printableLogMap = *logMap;
        EXPECT_TRUE(checkLogMapForValue(PortAuthority::LogSevarity::ERROR, *logMap,
            "Failed to open socket on port 2010")) << printableLogMap.dump(4);
    }

    TEST(StartApplication, will_print_an_information_level_log_stating_that_the_application_has_sucessfully_started)
    {
        //given we have a port authority
        PortAuthority authority(2000,2010,10);
        mutex loggerMutex;
        auto logMap = setLoggerOnPortAuthority(authority, loggerMutex);

        //when we start the application
        thread serverThread([&authority](){authority.startApplication();});
        waitForApplicationBootupThenPowerDown(loggerMutex,authority,*logMap);
        serverThread.join();

        //then the log will contain an info log stating that the application has started
        json printableLogMap = *logMap;
        EXPECT_TRUE(checkLogMapForValue(PortAuthority::LogSevarity::INFO, *logMap, "Server Started Successfully"))
            << printableLogMap.dump(4);
    }
}