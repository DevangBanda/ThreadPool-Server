#pragma once
#include "Socket.h"
#include <string>

class TcpListener {
public:
    bool listenOn(uint16_t port, int backlog = 128);
    Socket acceptOne();
    void close();

private:
    Socket server_;
};