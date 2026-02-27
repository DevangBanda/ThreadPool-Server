/*
A multithreaded HTTP/1.1 server implemented from scratch in modern C++
using POSIX sockets and a bounded thread pool.

Devang Banda
*/

#include <atomic>
#include <csignal>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "core/ThreadPool.h"
#include "net/TcpListener.h"
#include "http/HttpParser.h"
#include "http/Router.h"

static std::atomic<bool> g_stop{false};

static void onSigint(int) {
    g_stop.store(true);
}

static bool readHttpHeaders(int fd, std::string& out) {
    constexpr size_t MAX = 64 * 1024;
    char buf[4096];

    while (out.size() < MAX) {
        ssize_t n = ::recv(fd, buf, sizeof(buf), 0);
        if (n <= 0) return false;

        out.append(buf, buf + n);

        if (out.find("\r\n\r\n") != std::string::npos)
            return true;
    }
    return false;
}

static void writeAll(int fd, const std::string& data) {
    const char* p = data.data();
    size_t left = data.size();

    while (left > 0) {
        ssize_t n = ::send(fd, p, left, 0);
        if (n <= 0) return;

        p += n;
        left -= static_cast<size_t>(n);
    }
}

int main() {
    std::signal(SIGINT, onSigint);

    // -------- Setup Router --------
    auto router = std::make_shared<Router>();

    router->get("/health", [](const HttpRequest&) {
        HttpResponse r;
        r.headers["Content-Type"] = "text/plain";
        r.body = "ok\n";
        return r;
    });

    router->get("/", [](const HttpRequest&) {
        HttpResponse r;
        r.headers["Content-Type"] = "text/html; charset=utf-8";
        r.body = "<h1>hello from Devang Banda</h1>\n";
        return r;
    });

    // -------- Setup Listener --------
    TcpListener listener;
    if (!listener.listenOn(8080)) {
        std::cerr << "Failed to listen on 8080\n";
        return 1;
    }

    // -------- Setup Thread Pool --------
    ThreadPool pool(8, 1024);

    std::cout << "Listening on http://localhost:8080 (Ctrl+C to stop)\n";

    // -------- Accept Loop --------
    while (!g_stop.load()) {

        Socket client = listener.acceptOne();
        if (!client.valid()) {
            if (g_stop.load()) break;
            continue;
        }

        int cfd = client.fd();

        bool submitted = pool.submit([cfd, router]() {

            // RAII ownership transferred to worker thread
            Socket s{cfd};

            std::string raw;
            if (!readHttpHeaders(s.fd(), raw))
                return;

            HttpRequest req;
            if (!HttpParser::parseRequest(raw, req))
                return;

            HttpResponse resp = router->route(req);
            resp.headers["Connection"] = "close";

            writeAll(s.fd(), resp.toString());
        });

        if (!submitted) {
            // Queue full or shutting down — prevent fd leak
            ::close(cfd);
        } else {
            // Transfer ownership to worker
            client.reset(-1);
        }
    }

    // -------- Shutdown --------
    listener.close();
    pool.shutdown();

    std::cout << "Shutdown complete.\n";
    return 0;
}