//Implementation of the Thread Pool class

#include "ThreadPool.h"
using namespace std;

ThreadPool::ThreadPool(size_t threads, size_t queue_capacity)
    : q_(queue_capacity){
        started_ = true;    //Flag to control the shutdown logic
        workers_.reserve(threads);  //Prevents reallocation
        for(size_t i=0; i<threads; i++){
            workers_.emplace_back([this]{workerLoop();});   //construct a thread inside the vector
        }  
    }

ThreadPool::~ThreadPool()
{
    shutdown();
}

bool ThreadPool::submit(Job job)
{
    return q_.push(std::move(job));
}

void ThreadPool::shutdown(){
    if(!started_) return; 
    started_ = false; 
    q_.close(); 
    for(auto &t : workers_){
        if(t.joinable()) t.join();
    }
}

void ThreadPool::workerLoop(){
    Job job; 
    while(q_.pop(job)) job();
}