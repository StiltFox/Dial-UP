/*******************************************************
* Created by Cryos on 7/7/25.
* Copyright 2025 Stilt Fox® LLC
*
* See LICENSE on root project directory for terms
* of use.
********************************************************/
#include "ThreadPool.h++"

using namespace std;

namespace StiltFox::DialUp
{
    void ThreadPool::threadMain() const
    {
        while (!shutdown)
        {
            task();
        }
    }

    ThreadPool::ThreadPool(int maxThreads, const function<void()>& task)
    {
        shutdown = false;
        this->maxThreads = maxThreads;
        threads = new thread*[maxThreads];
        this->task = task;

        for (int x = 0; x < maxThreads; x++) threads[x] = new thread(threadMain);
    }

    ThreadPool::~ThreadPool()
    {
        shutdown = true;
        for (int x=0; x<maxThreads; x++)
        {
            threads[x]->join();
            delete threads[x];
        }
        delete[] threads;
    }
}