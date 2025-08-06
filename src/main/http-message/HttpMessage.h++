/*******************************************************
* Created by cryos on 6/5/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/

#ifndef Stilt_Fox_3482cf09b5224d89bc0997f795fff9e1
#define Stilt_Fox_3482cf09b5224d89bc0997f795fff9e1
#include <string>
#include <vector>
#include <unordered_map>
#include "Url.h++"

namespace StiltFox::DialUp
{
    /******************************************************************************************************************
     * This class is meant to contain a Http Message. This represents both requests and responses in one class because
     * they're both so similar.
     *****************************************************************************************************************/
    struct HttpMessage
    {
        /***************************************************************************************************************
         * This enum keeps track of all the different HttpMethods that can be accepted. There are two special cases
         * however that should be treated differently.
         *
         * - ERROR - This is intended to represent that an error has occurred during parsing. If a method in this class
         *           returns an HttpMessage with a method of ERROR, check the body for the malformed request that failed
         *           parsing. DO NOT USE THIS MANUALLY TO BUILD A REQUEST.
         * - NONE - This is to indicate that there is no HTTP method. This is just a default placeholder for building
         *          Http Responses, as responses do not use this field. DO NOT USE THIS TO MANUALLY BUILD A REQUEST.
         **************************************************************************************************************/
        enum Method {GET,HEAD,POST,PUT,PATCH,DELETE,CONNECT,OPTIONS,TRACE,ERROR,NONE};
        int statusCode = 0;
        Method httpMethod = NONE;
        Url requestUri;
        std::string httpVersion, body;
        std::unordered_map<std::string,std::vector<std::string>> headers;

        /***************************************************************************************************************
         * This constructor is used to create a Http Message from a vector of raw bytes. This is useful because things
         * like websockets typically return raw binary data. This can be used to parse either a request or a response.
         *
         * @param data - The data that you would like to parse into a Http Message. The data will not be modified in
         *               any way by this procedure.
         **************************************************************************************************************/
        HttpMessage(const std::vector<char>& data);
        /***************************************************************************************************************
         * This constructor is used to create a Http Response. This will set the Http Method to NONE.
         *
         * @param statusCode - A number representing the response status. Ie: 404, 200, ect...
         * @param headers - A map of headers, each one able to support multiple values. Defaults to an empty map.
         * @param body - A string representing the body of the message you wish to send. Defaults to an empty string.
         **************************************************************************************************************/
        HttpMessage(int statusCode, std::unordered_map<std::string,std::vector<std::string>> headers = {},
            std::string body = "");
        /***************************************************************************************************************
         * This constructor is used to create a Http Request. This will set the statusCode to zero.
         *
         * @param method - The method that our request will be using. Ie: GET, PUT, POST, ect...
         * @param uri - This is a string that represents the path of the url after the host name. IE: /admin
         *              defaults to "/".
         * @param headers - A map of headers, each one able to support multiple values. Defaults to empty map
         * @param body - A string representing the body of the message you wish to send. Defaults to an empty string.
         **************************************************************************************************************/
        HttpMessage(Method method, std::string uri = "/",
            std::unordered_map<std::string,std::vector<std::string>> headers = {}, std::string body = "");
        /***************************************************************************************************************
         * This constructor creates a deep copy of the message passed in.
         *
         * @param message = the message to copy the data from.
         **************************************************************************************************************/
        HttpMessage(const HttpMessage& message);
        /***************************************************************************************************************
         * This returns the Http Method as an upper case string representation. This is used to help with maps and such
         * as the Http Method is actually stored as an enum.
         *
         * @return returns the upper case string representation of the Http Method.
         **************************************************************************************************************/
        std::string getHttpMethodAsString() const;
        /***************************************************************************************************************
         * This method will print the Http Message as an Http Response. This will ignore any data that's only relevant
         * to a request. So let's say you have something like the Method set. This data will be ignored.
         *
         * @return returns the string representation of the data as an Http Response. Ignores all unnecessary data.
         **************************************************************************************************************/
        std::string printAsResponse() const;
        /***************************************************************************************************************
         * This method will print the Http Message as an Http Request. This will ignore any data that's only relevant
         * to a response. So let's say you have something like the status code set. This data will be ignored.
         *
         * @return returns the string representation of the data as an Http Request. Ignores all unnecessary data.
         **************************************************************************************************************/
        std::string printAsRequest() const;
        /***************************************************************************************************************
         * These methods overload the equality operators. These simply do a field by field comparison of the two objects
         * and then returns the results. The idea of a message being a request or a response is irrelevant here.
         **************************************************************************************************************/
        bool operator==(const HttpMessage&) const;
        bool operator!=(const HttpMessage&) const;

    protected:
        /***************************************************************************************************************
         * These methods are for internal use only. These will not be document like the other methods. If you wish to
         * understand them, because you're actively developing this class or extending it, it would be best for you to
         * familiarize yourself with the code.
         **************************************************************************************************************/
        std::string printBodyAndHeaders() const;
        void parse(const std::vector<char>& data);
    };
}

#endif