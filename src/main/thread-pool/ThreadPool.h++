/*******************************************************
* Created by cryos on 7/7/25.
* Copyright 2024 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#ifndef Stilt_Fox_ef4af34d562b405082d82599d0e55247
#define Stilt_Fox_ef4af34d562b405082d82599d0e55247
#include <functional>
#include <queue>
#include <thread>

namespace StiltFox::DialUp
{
    class ThreadPool
    {
        int maxThreads;
        std::function<void()> task;
        std::thread** threads;
        bool shutdown;

        void threadMain() const;
    public:
        ThreadPool(int maxThreads, const std::function<void()>& task);
        ~ThreadPool();
    };
}
#endif