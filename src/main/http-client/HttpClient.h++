/*******************************************************
* Created by Cryos on 4/20/26.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifndef Stilt_Fox_bdd7d03c2abf4083bd6c04e1ca6885be
#define Stilt_Fox_bdd7d03c2abf4083bd6c04e1ca6885be
#include <mutex>
#include "HttpMessage.h++"

namespace StiltFox::DialUp
{
  /*********************************************************************************************************************
   * This class is meant to enable you to call external APIs and other web resources. Doing this will require filling
   * out an HttpMessage with the URL, parameters, headers, body and so on that your API requires.
   *
   * This class is a singleton class. You are responsible for deleting it as your program finishes. This is because
   * the setup for curl library should happen one time per application.
   ********************************************************************************************************************/
  class HttpClient
  {
    /*******************************************************************************************************************
     * The static fields here contain the mutex and pointer used to controll access to this class. The mutex insures
     * that getInstance() is thread safe and that client dosnt get double instanciated.
     ******************************************************************************************************************/
    static std::mutex clientMutex;
    static HttpClient* client;

    /*******************************************************************************************************************
     * These constructors and operators are closed off to the user of this library to preserve the singleton aspect of
     * this class. This protects the internal curl calls from being setup multiple times.
     ******************************************************************************************************************/
    HttpClient();
    HttpClient(HttpClient&) = delete;
    HttpClient& operator=(HttpClient&) = delete;

  public:
    /*******************************************************************************************************************
     * This method is used to get an instance of HttpClient. If an instance already exists, you will be given a pointer
     * to the currently active instance. Deleting this pointer before the program is ready to exit will cause issues.
     *
     * Only delete this object once all threads have exited and at the end of execution of the program.
     *
     * @return returns the pointer to the global instance of HttpClient.
     ******************************************************************************************************************/
    static HttpClient* getInstance();
    /*******************************************************************************************************************
     * This method is used to actually send a web request to an external resource.
     *
     * @param request - This is the Http request that you wish to send.
     * @return returns the Http response from the external resource.
     ******************************************************************************************************************/
    HttpMessage sendWebRequest(const HttpMessage& request);

    /*******************************************************************************************************************
     * This method will delete the client pointer and close the curl runtime setup.
     *  - THIS IS NOT THREAD SAFE!!!
     *  - DO NOT DELETE THE POINTER UNTILL ALL OTHER THREADS ARE CLOSED!!!
     *  - ONLY DELETE FROM THE MAIN THREAD AT END OF PROGRAM EXECUTION!!!
     * Obey these rules and you will be safe.
     ******************************************************************************************************************/
    ~HttpClient();
  };
}

#endif
