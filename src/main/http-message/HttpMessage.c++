/*******************************************************
* Created by Cryos on 6/5/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <stack>
#include <cctype>
#include "HttpMessage.h++"

using namespace std;

namespace StiltFox::DialUp
{
    inline string getReasonCode(int statusCode)
    {
        unordered_map<int,string> statusCodes = {{100, "Continue"},{101, "Switching Protocols"},
            {102,"Processing"},{103, "Early Hints"},{200,"OK"},{201,"Created"},{202,"Accepted"},
            {203,"Non-Authoritative Information"},{204,"No Content"},{205,"Reset Content"},
            {206,"Partial Content"},{207,"Multi-Status"},{208,"Already Reported"},{226,"IM Used"},
            {300,"Multiple Choices"},{301,"Moved Permanently"},{302,"Found"},{303,"See Other"},
            {304,"Not Modified"},{305,"use proxy"},{306,"unused"},{307,"Temporary Redirect"},
            {308,"Permanent Redirect"},{400,"Bad Request"},{401,"Unauthorized"},
            {402,"Payment Required"},{403,"Forbidden"},{404,"Not Found"},{405,"Method Not Allowed"},
            {406,"Not Acceptable"},{407,"Proxy Authentication Required"},{408,"Request Timeout"},
            {409,"Conflict"},{410,"Gone"},{411,"Length Required"},{412,"Precondition Failed"},
            {413,"Payload Too large"},{414,"URI Too Long"},{415,"Unsupported Media Type"},
            {416,"Range Not Satisfiable"},{417,"Expectation Failed"},{418,"I'm a teapot"},
            {421,"Misdirected Request"},{422,"Unprocessable Entity"},{423,"Locked"},
            {424,"Failed Dependency"},{425,"Too Early"},{426,"Upgrade Required"},
            {428,"Precondition Required"},{429,"Too Many Requests"},{431,"Request Header Fields Too Large"},
            {451, "Unavailable For Legal Reasons"},{500,"Internal Server Error"},{501,"Not Implemented"},
            {502,"Bad Gateway"},{503,"Service Unavailable"},{504,"Gateway Timeout"},
            {505,"HTTP Version Not Supported"},{506,"Variant Also Negotiates"},{507, "Insufficient Storage"},
            {508,"Loop Detected"},{510,"Not Extended"},{511,"Network Authentication Required"}};

        return statusCodes.contains(statusCode) ? statusCodes[statusCode] : "";
    }

    inline string getStringMethod(HttpMessage::Method method)
    {
        unordered_map<HttpMessage::Method,string> methodStrings = {{HttpMessage::Method::GET, "GET"},
            {HttpMessage::Method::HEAD, "HEAD"},{HttpMessage::Method::POST, "POST"},
            {HttpMessage::Method::PUT, "PUT"},{HttpMessage::Method::PATCH, "PATCH"},
            {HttpMessage::Method::DELETE, "DELETE"},{HttpMessage::Method::CONNECT, "CONNECT"},
            {HttpMessage::Method::OPTIONS, "OPTIONS"},{HttpMessage::Method::TRACE, "TRACE"}};

        return methodStrings.contains(method) ? methodStrings[method] : "";
    }

    inline HttpMessage::Method getMethodFromString(const string& method)
    {
        unordered_map<string,HttpMessage::Method> stringMethods = {{"GET", HttpMessage::Method::GET},
            {"HEAD", HttpMessage::Method::HEAD},{"POST", HttpMessage::Method::POST},
            {"PUT", HttpMessage::Method::PUT},{"PATCH", HttpMessage::Method::PATCH},
            {"DELETE", HttpMessage::Method::DELETE},{"CONNECT", HttpMessage::Method::CONNECT},
            {"OPTIONS", HttpMessage::Method::OPTIONS},{"TRACE", HttpMessage::Method::TRACE}};

        return stringMethods.contains(method) ? stringMethods[method] : HttpMessage::Method::ERROR;
    }

    inline string parseToWhitespace(stack<char, vector<char>>& buffer)
    {
        string output;

        while (!buffer.empty() && !isspace(buffer.top()))
        {
            output.push_back(buffer.top());
            buffer.pop();
        }

        return output;
    }

    HttpMessage::HttpMessage(int status, unordered_map<string,vector<string>> head, string bod)
    {
        statusCode = status;
        httpMethod = NONE;
        httpVersion = "HTTP/1.1";
        headers = move(head);
        body = move(bod);
    }

    HttpMessage::HttpMessage(Method method, string uri, unordered_map<string,vector<string>> head, string bod)
    {
        statusCode = 0;
        httpMethod = method;
        httpVersion = "HTTP/1.1";
        requestUri = move(uri);
        headers = move(head);
        body = move(bod);
    }

    HttpMessage::HttpMessage(const vector<char>& data)
    {
        stack parsingBuffer(data);

        //remove whitespace before message if any
        while (!parsingBuffer.empty() && isspace(parsingBuffer.top())) parsingBuffer.pop();

        httpMethod = getMethodFromString(parseToWhitespace(parsingBuffer));
        parsingBuffer.pop();
        requestUri = parseToWhitespace(parsingBuffer);
        parsingBuffer.pop();
        httpVersion = parseToWhitespace(parsingBuffer);
    }

    HttpMessage::HttpMessage(const HttpMessage& messageToCopy)
    {
        statusCode = messageToCopy.statusCode;
        httpMethod = messageToCopy.httpMethod;
        requestUri = messageToCopy.requestUri;
        httpVersion = messageToCopy.httpVersion;
        headers = messageToCopy.headers;
        body = messageToCopy.body;
    }

    string HttpMessage::printBodyAndHeaders() const
    {
        string output;

        for (const auto & [key, value] : headers)
        {
            output += key;
            output += ": ";

            for (const auto& trait: value)
            {
                string excapedTrait = trait.find(',') != string::npos ? "\"" + trait + "\"" : trait;
            }

            output += "\r\n";
        }

        return output + "\r\n" + body;
    }

    string HttpMessage::printAsResponse() const
    {
        return httpVersion + " " + to_string(statusCode) + " " + statusReason+"\r\n" + printBodyAndHeaders();
    }

    string HttpMessage::printAsRequest() const
    {
        return getStringMethod(httpMethod) + " " + requestUri +" " + httpVersion + "\r\n" + printBodyAndHeaders();
    }

    string HttpMessage::getHttpMethodAsString() const
    {
        return getStringMethod(httpMethod);
    }

    bool HttpMessage::operator==(const HttpMessage& other) const
    {
        return statusCode == other.statusCode && httpMethod == other.httpMethod && requestUri == other.requestUri
               && headers == other.headers && body == other.body && httpVersion == other.httpVersion;
    }

    bool HttpMessage::operator!=(const HttpMessage& other) const
    {
        return !(statusCode == other.statusCode && httpMethod == other.httpMethod && requestUri == other.requestUri
                 && headers == other.headers && body == other.body && httpVersion == other.httpVersion);
    }
}

// void HttpMessage::parseString(queue<char>& request)
// {
//     statusCode = 0;
//     requestUri = parseToDelim(request, ' ', '\n');
//     requestUri.starts_with("HTTP") ? requestUri = "" : parseToDelim(request, '\n');
//
//     while (!request.empty() && request.front() != '\r' && request.front() != '\n')
//     {
//         string key = parseToDelim(request, ':');
//         if (request.front() == ' ') request.pop();
//         headers[key] = parseToDelim(request, '\r');
//         if (request.front() == '\n') request.pop();
//     }
//     if (request.front() == '\r') parseToDelim(request, '\n');
//
//     body = parseToDelim(request, '\000');
// }

// inline string parseToDelim(queue<char>& toParse, char delim, char backupDelim)
// {
//     string output;
//
//     while (!toParse.empty() && toParse.front() != delim && toParse.front() != backupDelim)
//     {
//         output += toParse.front();
//         toParse.pop();
//     }
//     if (!toParse.empty()) toParse.pop();
//
//     return output;
// }
//
// inline string parseToDelim(queue<char>& toParse, char delim)
// {
//     return parseToDelim(toParse, delim, '\000');
// }