#pragma once
#include <condition_variable>
#include <mutex> 
#include <queue> 

//Blocking Queue for producer-consumer asynch design

template <typename T> 
class BlockingQueue {
    public :

    explicit BlockingQueue(size_t capacity) : capacity_(capacity){} 

    //Member Function for producer threads
    // Acquire lock, wait if the queue is full, add item to q_, wake up consumer
    bool push(T item)
    {
        std::unique_lock<std::mutex> lk(mu_); //can unlock, relock automatically
        not_full_.wait(lk, [&]{return closed_ || q_.size() < capacity_;});
        if(closed_) return false; 
        q_.push(std::move(item)); // move semantics 
        not_empty_.notify_one();
        return true;    //push succeeded
    }

    //Member function called by consumer threads
    bool pop(T& out)
    {
        std::unique_lock<std::mutex> lk(mu_);//Acquire a mutex lock
        not_empty_.wait(lk, [&]{return closed_ || !q_.empty();});   //Wait while the predicate is false(closed or the q is empty)
        if(q_.empty()) return false; 
        out = std::move(q_.front());
        q_.pop(); 
        not_full_.notify_one();
        return true;
    }

    void close()
    {
        std::lock_guard<std::mutex> lk(mu_);
        closed_ = true; 
        not_empty_.notify_all();    //Wake all consumer threads
        not_full_.notify_all();     //Wake all producer threads
    }

    private: 
        std::mutex mu_;
        std::condition_variable not_empty_, not_full_; 
        std::queue<T> q_; 
        size_t capacity_; 
        bool closed_{false};
};