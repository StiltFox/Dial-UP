#include <gtest/gtest.h>
#include <sstream>
#include <Stilt_Fox/UniversalLibrary/CoutCapture.hpp>
#include <Stilt_Fox/UniversalLibrary/TempFile.hpp>
#include "StiltFox-Web-Framework.hpp"

using namespace std;
using namespace StiltFox::Web::Framework;
using namespace StiltFox::UniversalLibrary;

string expectedHeader = "\n"
                  "  @@@@@@  @@@@@@@@ @@@@ @@       @@@@@@@@    @@@@@@@@  @@@@@@@  @@     @@\n"
                  " @@    @@    @@     @@  @@          @@       @@       @@     @@  @@   @@ \n"
                  " @@          @@     @@  @@          @@       @@       @@     @@   @@ @@  \n"
                  "  @@@@@@     @@     @@  @@          @@       @@@@@@   @@     @@    @@@   \n"
                  "       @@    @@     @@  @@          @@       @@       @@     @@   @@ @@  \n"
                  " @@    @@    @@     @@  @@          @@       @@       @@     @@  @@   @@ \n"
                  "  @@@@@@     @@    @@@@ @@@@@@@@    @@       @@        @@@@@@@  @@     @@\n\n"
                  "                            #                                   \n"
                  "                           @@@@@#                               \n"
                  "          @@@/            %@@@@@@@@                             \n"
                  "          &@@@@@@@%       @@@@@@@@@@@          @           /    \n"
                  "           @@@@@@@@@@@&  (@@@@@@@@@@@@%      @@@  #@@@@@@@@     \n"
                  "           /@@@@@@@@@@@@@@@@@@@@@@@@@@@% #@@@@@@@@@@@@@@@/      \n"
                  "            #@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      \n"
                  "             %@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@(    \n"
                  "              /@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   \n"
                  "                @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#\n"
                  "                 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  \n"
                  "                 #@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \n"
                  "                  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&\n"
                  "                   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \n"
                  "                  (@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \n"
                  "                 #@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  #@\n"
                  "                #@@@@@@@@@@@@@   %@@@@@@@@@@@@@@@@@@@@@@@@@@    \n"
                  "                /@@@@@@@@@&       @@@@@@@@@@@@@@@@@@@@@@@@@     \n"
                  "                   @@@/            @@@@@@@@@@@@@@@@@@@%@@@      \n"
                  "                                  @@@@@@@@@@@@@@@@@@@@   %      \n"
                  "                               /@@&    @@@@@@@@@@@@@@@@         \n"
                  "                                       @ %@@(#@@@@@@   /@       \n"
                  "                                           @    (@@%            \n"
                  "                                                  @             \n"
                  "https://www.stiltfox.com\n"
                  "https://donate.stiltfox.com\n";


TEST(WebApp, start_will_print_stilt_fox_logo_to_console_by_default)
{
    //given we have a web application
    CoutCapture output;
    WebApp testApp("");

    //when we start the application
    testApp.start();

    //then we get back our logo
    ASSERT_EQ(output.getOutput(), expectedHeader);
}

TEST(WebApp, start_will_print_logs_to_a_file_if_the_option_is_set)
{
    //given we have a file that we want to print logs to and we have a config file
    CoutCapture output;
    TemporaryFile logFile(".webapp_temp_00");
    TemporaryFile configFile(".webapp_tmp_00_config", "{\"logFile\": \"" + logFile.getPath() + "\"}");

    //when we start the application with the config file
    WebApp testApp(configFile.getPath());
    testApp.start();

    //then the logs are printed to the file
    ASSERT_EQ(logFile.read(), expectedHeader);
    ASSERT_EQ(output.getOutput(), "");
}

/*
TEST(WebApp, Constructing_WebApp_will_report_if_the_configuration_cannot_be_found)
{
    //given we have a config path that cannot be found
    CoutCapture output;
    File file = ".WebApp_does_not_exist";
    
    //when we initialize the web app with a file that does not exist
    WebApp application(file.getPath());

    //then we do not create the file and the missing file is reported
    ASSERT_FALSE(file.exists());
    ASSERT_TRUE(output.capture.str().starts_with("\ncould not read config file at: " + file.getPath()));
}*/