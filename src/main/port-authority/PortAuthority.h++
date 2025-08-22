/*******************************************************
* Created by cryos on 6/20/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifndef Stilt_Fox_65d185776f6249aaa0182b82cad0384b
#define Stilt_Fox_65d185776f6249aaa0182b82cad0384b
#include <memory>
#include <thread>
#include <functional>
#include "EndpointRegistry.h++"
#include "ServerSocket.h++"
#include "StiltFoxAsciiArt.h++"
#include "ClientConnection.h++"

namespace StiltFox::DialUp
{
    /*******************************************************************************************************************
     * This is the main class that will run the web portion of your http based api. This program library as of writing
     * will only accept http requests and requires being behind a proxy engine if you want something like https.
     *
     * Typically, when making a http based web application, this is the only class you should directly interact with.
     ******************************************************************************************************************/
    class PortAuthority
    {
        std::mutex threadCountMutex;
        int maxThreads, currentThreads;
        long maxWaitTime, maxDataSize;
        std::shared_ptr<ServerSocket> socket, killSocket;

        void startMainLoop();
        void startWorkerThread(std::shared_ptr<ClientConnection> connection, Response data);

    public:
        /***************************************************************************************************************
         * This member variable stores the accepted kill message that needs to be sent to the kill port in order to
         * stop the application. The kill port will be set when the PortAuthority class is instantiated.
         **************************************************************************************************************/
        static const HttpMessage KILL_MESSAGE;
        /***************************************************************************************************************
         * This enum keeps track of all the different log types that can be used. While the class itself only uses
         * INFO and ERROR, the rest are here for your use, when writing your web app that utilizes this class.
         **************************************************************************************************************/
        enum class LogSevarity{DEBUG, INFO, WARNING, ERROR};

        /***************************************************************************************************************
         * Just access this member directly to register your classes. Documentation for this class is on the associated
         * header file.
         **************************************************************************************************************/
        EndpointRegistry registry;
        /***************************************************************************************************************
         * This is the function that will be used to log statuses about the application. By default, this function is
         * not thread safe and simply prints to cout. You may replace this function with something else, to make it
         * conform to the rest of your application.
         *
         * It is strongly recommended that you use mutexes and make your logging implementation thread safe if you plan
         * on calling it from your endpoints as well as here. You could just also lock a mutex before calling the
         * default logger in your endpoints, as this class will have completed logging by the time your code gets
         * called.
         **************************************************************************************************************/
        std::function<void(LogSevarity, std::string)> logger;

        /***************************************************************************************************************
         * This constructor will create a PortAuthority object and do nothing else. It will not block the thread or
         * start accepting requests yet.
         *
         * @param portNumber - This is the port number you would like your main application to listen to. This port must
         *                     not be in use. Once this class locks a port number it will close it off to sharing.
         * @param killPortNumber - This is the port number that will listen for KILL_MESSAGE. Once the message is
         *                         received, the sockets and all worker threads will be terminated. PortAuthority will
         *                         stop blocking the thread if startApplication has been called.
         * @param maxWorkerThreads - This is the amount of threads that can be serving web requests at one time. Any
         *                           connections beyond this will have to wait for another request to stop processing.
         * @param maxWaitTime - This is the amount of time in milliseconds that the server will wait for a request to
         *                      finish transmitting. If time is reached an error will be sent to the client and the
         *                      connection will be closed.
         * @param maxDataSize - This is the amount of data that can be transmitted before the server will error. If the
         *                      data limit is reached by a request an error will be sent to the client and the
         *                      connection will be terminated.
         **************************************************************************************************************/
        PortAuthority(int portNumber, int killPortNumber, int maxWorkerThreads, long maxWaitTime=10000,
            long maxDataSize=80000000);
        /***************************************************************************************************************
         * Removing the copy constructor. This is done because many things like threads and sockets and such do not
         * make sense to copy and could cause confusion. To prevent this from happening, a new PortAuthority must be
         * constructed wherever you would like to use one.
         **************************************************************************************************************/
        PortAuthority(const PortAuthority&) = delete;

        /***************************************************************************************************************
         * This function will block the current thread and open up connection ports. Web services will start and
         * received http messages will start being routed to the proper endpoints.
         *
         * @param banner - This is the custom banner that gets printed to console before boot up. To turn this feature
         *                 off entirely, simply set this value to "".
         **************************************************************************************************************/
        void startApplication(const std::string& banner = DEFAULT_BANNER);
        /***************************************************************************************************************
         * This function will stop the PortAuthority, unblock the threads it's blocking and cease all processing of web
         * traffic. Obviously, you cannot call this from a blocked thread. So if you intend to use this plan wisely.
         **************************************************************************************************************/
        void stopApplication();

        /***************************************************************************************************************
         * This deconstructor will simply call stopApplication. This insures that your program will clean up after
         * itself once it goes out of scope.
         **************************************************************************************************************/
        ~PortAuthority();
    };
}
#endif