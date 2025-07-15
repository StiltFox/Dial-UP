/*******************************************************
* Created by Cryos on 6/5/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <queue>
#include <cctype>
#include "HttpMessage.h++"

#include <locale>

using namespace std;

namespace StiltFox::DialUp
{
    inline string getReasonCode(int statusCode)
    {
        unordered_map<int,string> statusCodes = {{100,"Continue"},{101,"Switching Protocols"},
            {102,"Processing"},{103,"Early Hints"},{200,"OK"},{201,"Created"},{202,"Accepted"},
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
            {451,"Unavailable For Legal Reasons"},{500,"Internal Server Error"},{501,"Not Implemented"},
            {502,"Bad Gateway"},{503,"Service Unavailable"},{504,"Gateway Timeout"},
            {505,"HTTP Version Not Supported"},{506,"Variant Also Negotiates"},{507,"Insufficient Storage"},
            {508,"Loop Detected"},{510,"Not Extended"},{511,"Network Authentication Required"}};

        return statusCodes.contains(statusCode) ? statusCodes[statusCode] : "";
    }

    inline string getStringMethod(HttpMessage::Method method)
    {
        unordered_map<HttpMessage::Method,string> methodStrings = {{HttpMessage::Method::GET, "GET"},
            {HttpMessage::Method::HEAD, "HEAD"},{HttpMessage::Method::POST, "POST"},
            {HttpMessage::Method::PUT, "PUT"},{HttpMessage::Method::PATCH, "PATCH"},
            {HttpMessage::Method::DELETE, "DELETE"},{HttpMessage::Method::CONNECT, "CONNECT"},
            {HttpMessage::Method::OPTIONS, "OPTIONS"},{HttpMessage::Method::TRACE, "TRACE"},
            {HttpMessage::Method::ERROR,"ERROR"}};

        return methodStrings.contains(method) ? methodStrings[method] : "";
    }

    inline HttpMessage::Method getMethodFromString(const string& method)
    {
        unordered_map<string,HttpMessage::Method> stringMethods = {{"GET", HttpMessage::Method::GET},
            {"HEAD", HttpMessage::Method::HEAD},{"POST", HttpMessage::Method::POST},
            {"PUT", HttpMessage::Method::PUT},{"PATCH", HttpMessage::Method::PATCH},
            {"DELETE", HttpMessage::Method::DELETE},{"CONNECT", HttpMessage::Method::CONNECT},
            {"OPTIONS", HttpMessage::Method::OPTIONS},{"TRACE", HttpMessage::Method::TRACE},
            {"ERROR", HttpMessage::Method::ERROR}};

        return stringMethods.contains(method) ? stringMethods[method] : HttpMessage::Method::ERROR;
    }

    inline string parseToDelim(queue<char>& toParse, char delim, char backupDelim)
    {
        string output;

        while (!toParse.empty() && toParse.front() != delim && toParse.front() != backupDelim)
        {
            output += toParse.front();
            toParse.pop();
        }
        if (!toParse.empty()) toParse.pop();

        return output;
    }

    inline string parseToDelim(queue<char>& toParse, char delim)
    {
        return parseToDelim(toParse, delim, '\000');
    }

    inline void trimLeadingSpaces(queue<char>& toParse)
    {
        while (!toParse.empty() && isspace(toParse.front())) toParse.pop();
    }

    inline vector<string> parsePotentialMultiValueHeader(const string& value, vector<string>& output)
    {
        queue toParse(deque(value.begin(), value.end()));

        while (!toParse.empty())
        {
            output.push_back(parseToDelim(toParse, ',', '\0'));
            if (!toParse.empty() && toParse.front() == ' ') toParse.pop();
        }

        return output;
    }

    inline unordered_map<string, vector<string>> parseHeaders(queue<char>& toParse)
    {
        unordered_map<string, vector<string>> headers;

        while (!toParse.empty() && toParse.front() != '\r' && toParse.front() != '\n')
        {
            string key = parseToDelim(toParse, ':');
            if (toParse.front() == ' ') toParse.pop();
            string headerValues = parseToDelim(toParse, '\r');
            parsePotentialMultiValueHeader(headerValues, headers[key]);
            if (toParse.front() == '\n') toParse.pop();
        }

        return headers;
    }

    inline queue<char> getLine(queue<char>& toParse)
    {
        trimLeadingSpaces(toParse);
        string outputLine = parseToDelim(toParse, '\r', '\n');
        if (!toParse.empty() && toParse.front() == '\n') toParse.pop();

        return queue(deque(outputLine.begin(), outputLine.end()));
    }

    inline void parseFirstLine(HttpMessage* toWriteTo, queue<char>& toParse)
    {
        if (toWriteTo != nullptr)
        {
            string token = parseToDelim(toParse, ' ');

            if (token.starts_with("HTTP"))
            {
                toWriteTo->httpVersion = token;
                token = parseToDelim(toParse, ' ');

                try
                {
                    toWriteTo->statusCode = stoi(token);
                }
                catch(...)
                {
                    toWriteTo->statusCode = 0;
                }
                //ignore the rest, status reason is not needed.
            }
            else
            {
                toWriteTo->httpMethod = getMethodFromString(token);
                toWriteTo->requestUri = parseToDelim(toParse, ' ');
                toWriteTo->httpVersion = parseToDelim(toParse, ' ');
            }
        }
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

    HttpMessage::HttpMessage(const vector<char>& request)
    {
        parse(request);
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

    void HttpMessage::parse(const std::vector<char>& request)
    {
        queue parsingBuffer(deque(request.begin(), request.end()));
        queue<char> firstLine = getLine(parsingBuffer);

        parseFirstLine(this, firstLine);
        headers = parseHeaders(parsingBuffer);

        if (!parsingBuffer.empty() && parsingBuffer.front() == '\r') parseToDelim(parsingBuffer, '\n');
        body = httpMethod == ERROR ? string(request.begin(), request.end()) : parseToDelim(parsingBuffer, '\000');
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
        return httpVersion + " " + to_string(statusCode) + " " + getReasonCode(statusCode) + "\r\n" +
            printBodyAndHeaders();
    }

    string HttpMessage::printAsRequest() const
    {
        return getStringMethod(httpMethod) + " " + requestUri + " " + httpVersion + "\r\n" + printBodyAndHeaders();
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