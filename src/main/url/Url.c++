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
#include "Url.h++"

using namespace std;

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
        stringstream hex;
        char percentMode = -1;

        for (const unsigned char character : str)
        {
            if (percentMode == -1)
            {
                if (character == '%')
                {
                    percentMode = 0;
                }
                else
                {
                    output << character;
                }
            }
            else
            {
                percentMode++;
                hex << character;

                if (percentMode == 2)
                {
                    percentMode = -1;
                    char decoded = stoi(hex.str(), nullptr, 16);
                    output << decoded;
                }
            }
        }

        return output.str;
    }

    Url::Url(const vector<string>& pathSegments, const unordered_map<string, string>& parameters)
    {
        this->pathSegments = pathSegments;
        this->parameters = parameters;
    }

    string Url::toUrl() const
    {
        stringstream url;

        for(const string& segment : pathSegments) url << '/' << urlEncode(segment);

        if (!parameters.empty())
        {
            url << '?';
            for (const auto& parameter : parameters)
            {
                url << urlEncode(parameter.first) << "=" << urlEncode(parameter.second) << '&';
            }
        }

        string cleanedUrl = url.str();
        if (cleanedUrl.ends_with('&')) cleanedUrl.pop_back();

        return cleanedUrl;
    }

    string Url::toUrlWithoutParameters() const
    {
        stringstream url;

        for(const string& segment : pathSegments) url << '/' << urlEncode(segment);

        return url.str();
    }

    shared_ptr<Url> Url::parse(const string& url)
    {
        auto output = make_shared<Url>();

        if (!url.empty())
        {
            output->pathSegments = StandMixer::DataProcessor::tokenize(url,"/");
            const auto queryStartPosition = output->pathSegments.back().find_first_of("?");
            if (queryStartPosition != string::npos)
            {
                string parameters = output->pathSegments.back().substr(queryStartPosition+1);
                output->pathSegments.back() = output->pathSegments.back().substr(0, queryStartPosition);
                vector<string> parameterExpressions = StandMixer::DataProcessor::tokenize(parameters, "&");

                for (const auto expression : parameterExpressions)
                {
                    vector<string> tokenized = StandMixer::DataProcessor::tokenize(expression, "=");
                    output->parameters[tokenized[0]] = tokenized[1];
                }
            }
        }

        return output;
    }
}