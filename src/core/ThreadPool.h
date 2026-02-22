// Interface of Thread Pool class
//This class is used to manage a queue of jobs, set of worker threads, and startup and
//shutdown logic.

#pragma once
#include <functional>
#include <thread>
#include <vector>
#include "BlockingQueue.h"
using namespace std;

class ThreadPool{
    public:
        using Job = function<void()>; 

        ThreadPool(size_t threads, size_t queue_capacity);
        ~ThreadPool();

        bool submit(Job job);
        void shutdown();

    private:    
        void workerLoop(); 
        BlockingQueue<Job> q_; 
        vector<thread> workers_;
        bool started_ = false;
};