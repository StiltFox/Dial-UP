/*******************************************************
* Created by Cryos on 7/22/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <sstream>
#include <iomanip>
#include <Stilt_Fox/StandMixer/DataProcessor.h++>
#include <Stilt_Fox/StandMixer/DataConverter.h++>
#include <utility>
#include "Url.h++"

using namespace std;
using namespace StiltFox::StandMixer;

namespace StiltFox::DialUp
{
    inline string urlEncode(const string& str)
    {
        stringstream output;
        output << hex;

        for (const unsigned char character : str)
        {
            if (isalnum(character) || character == '-' || character == '_' || character == '.' || character == '~')
            {
                output << character;
            }
            else
            {
                output << uppercase;
                output << '%' << setw(2) << (int)character;
                output << nouppercase;
            }
        }

        return output.str();
    }

    inline string urlDecode(const string& str)
    {
        stringstream output;
        auto tokens = DataProcessor::tokenize(str, "%");

        if (!str.starts_with('%')) output << tokens[0];
        tokens.erase(tokens.begin());

        for (const auto& token : tokens)
        {
            if (token.size() >= 2)
            {
                auto decodedData = DataConverter::convertHexStringToData(token.substr(0, 2));
                for (const auto& character : decodedData) output << character;
                output << token.substr(2);
            }
            else
            {
                output << "%" << token;
            }
        }

        return output.str();
    }

    inline void printUrlWithoutParametersToStream(const Url& url, stringstream& output)
    {
        if (!url.host.empty())
        {
            if (!url.protocol.empty()) output << urlEncode(url.protocol) << "://";
            output << urlEncode(url.host);
            if (url.port > 0) output << ":" << url.port;
        }
        else if (url.pathSegments.empty())
        {
            output << "/";
        }

        for(const string& segment : url.pathSegments) output << '/' << urlEncode(segment);
    }

    Url::Url(std::string protocol, std::string host, int port, std::vector<std::string> pathSegments,
        std::unordered_map<std::string, std::string> parameters)
    {
        this->protocol = move(protocol);
        this->host = move(host);
        this->port = port;
        this->pathSegments = move(pathSegments);
        this->parameters = move(parameters);
    }

    string Url::toUrl() const
    {
        stringstream url;

        printUrlWithoutParametersToStream(*this, url);

        if (!parameters.empty())
        {
            url << '?';
            for (const auto& parameter : parameters)
                url << urlEncode(parameter.first) << "=" << urlEncode(parameter.second) << '&';
        }

        string cleanedUrl = url.str();
        if (cleanedUrl.ends_with('&')) cleanedUrl.pop_back();

        return cleanedUrl;
    }

    string Url::toUrlWithoutParameters() const
    {
        stringstream url;

        printUrlWithoutParametersToStream(*this, url);

        return url.str();
    }

    Url Url::parse(const string& url)
    {
        Url output;

        if (!url.empty())
        {
            queue data(deque(url.begin(), url.end()));

            if (url.find("://") != string::npos)
                output.protocol = DataProcessor::parseToDelimiter(data, "://");
            const auto hostInformation = DataProcessor::tokenize(DataProcessor::parseToDelimiter(data,"/"),":");

            if (!hostInformation.empty())
            {
                output.host = hostInformation[0];
                try
                {
                   if (hostInformation.size() > 1) output.port = stoi(hostInformation[1]);
                }
                catch (...)
                {
                    output.port = -2;
                }
            }

            output.pathSegments = DataProcessor::tokenize(DataProcessor::parseToDelimiter(data, "?"), "/");
            for (auto& segment : output.pathSegments) segment = urlDecode(segment);
            if (!data.empty())
            {
                vector<string> parameterExpressions = DataProcessor::tokenize(data, "&");
                for (const auto& expression : parameterExpressions)
                {
                    vector<string> tokenized = DataProcessor::tokenize(expression, "=");
                    output.parameters[urlDecode(tokenized[0])] = urlDecode(tokenized[1]);
                }
            }
        }

        return output;
    }

    bool Url::operator==(const Url& other) const
    {
        return pathSegments == other.pathSegments && parameters == other.parameters && port == other.port &&
               protocol == other.protocol && host == other.host;
    }

    bool Url::operator!=(const Url& other) const
    {
        return !(pathSegments == other.pathSegments && parameters == other.parameters && port == other.port &&
               protocol == other.protocol && host == other.host);
    }

    bool Url::operator==(const string& other) const
    {
        return parse(other) == *this;
    }

    bool Url::operator!=(const string& other) const
    {
        return *this != parse(other);
    }
}