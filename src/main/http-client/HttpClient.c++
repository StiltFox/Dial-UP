/*******************************************************
* Created by Cryos on 4/20/26.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include <curl/curl.h>
#include <sstream>
#include "HttpClient.h++"

using namespace std;

namespace StiltFox::DialUp
{
  std::mutex HttpClient::clientMutex;
  HttpClient* HttpClient::client = nullptr;

  size_t write_callback(char *data, size_t sizeInBytes, size_t numberOfItems, void* stringStream)
  {
    auto *output = (stringstream*)stringStream;
    for (int x=0; x<numberOfItems; x++) *output << data[x];

    return sizeInBytes * numberOfItems;
  }

  void sendCurlRequest(CURL *curl, const HttpMessage& request, stringstream& bodyBuffer)
  {
    curl_slist* headers = NULL;

    curl_easy_setopt(curl, CURLOPT_URL, request.requestUri.toUrl().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bodyBuffer);

    for (const auto& [name, values] : request.headers)
    {
      string strHeader = name + ": ";
      for (const auto& value : values) strHeader += value + ",";
      strHeader.pop_back();

      headers = curl_slist_append(headers, strHeader.c_str());
    }

    if (!request.body.empty()) curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.body.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request.getHttpMethodAsString().c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_perform(curl);
    curl_slist_free_all(headers);
  }

  HttpMessage getCurlResponse(CURL *curl, const stringstream& bodyBuffer)
  {
    HttpMessage output(500, {}, bodyBuffer.str());
    curl_header* header;
    curl_header* prev = nullptr;

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &output.statusCode);
    while ((header = curl_easy_nextheader(curl, CURLH_HEADER, 0, prev))) {
      output.headers[header->name] = {header->value};
      prev = header;
    }

    return output;
  }

  HttpClient::HttpClient()
  {
    curl_global_init(CURL_GLOBAL_ALL);
  }

  HttpClient* HttpClient::getInstance()
  {
    const lock_guard<mutex> lock(clientMutex);
    if (client == nullptr) client = new HttpClient();
    return client;
  }

  HttpMessage HttpClient::sendWebRequest(const HttpMessage& request)
  {
    HttpMessage output(500, {}, "curl not initialized");
    const auto curl = curl_easy_init();
    stringstream body;

    if (curl)
    {
      sendCurlRequest(curl, request, body);
      output = getCurlResponse(curl, body);
      curl_easy_cleanup(curl);
    }

    return output;
  }

  HttpClient::~HttpClient()
  {
    if(client != nullptr)
    {
      delete client;
      client = nullptr;
    }
    curl_global_cleanup();
  }
}
