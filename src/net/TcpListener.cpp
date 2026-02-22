#include "TcpListener.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>

bool TcpListener::listenOn(uint16_t port, int backlog) {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return false;

    int yes = 1;
    ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (::bind(fd, (sockaddr*)&addr, sizeof(addr)) < 0) { ::close(fd); return false; }
    if (::listen(fd, backlog) < 0) { ::close(fd); return false; }

    server_.reset(fd);
    return true;
}

Socket TcpListener::acceptOne() {
    int cfd = ::accept(server_.fd(), nullptr, nullptr);
    if (cfd < 0) return Socket{};
    return Socket{cfd};
}

void TcpListener::close() {
    server_.reset();
}