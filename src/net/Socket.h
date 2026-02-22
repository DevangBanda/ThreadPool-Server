#pragma once
#include <unistd.h>
using namespace std;

class Socket{
    public: 
        Socket() = default; 
        explicit Socket(int fd) : fd_(fd){}
        ~Socket() {reset();}

        //Move constructor
        Socket(Socket&& other) noexcept : fd_(other.fd_) { other.fd_ = -1; }
        
        //Move assignment 
        Socket& operator=(Socket&& other) noexcept {
            if (this != &other) { reset(); fd_ = other.fd_; other.fd_ = -1; }
            return *this;
        }

        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

        int fd() const { return fd_; }
        bool valid() const { return fd_ >= 0; }

        void reset(int newfd = -1) {
            if (fd_ >= 0) ::close(fd_);
            fd_ = newfd;
        }

    private:
        int fd_{-1}; 
};