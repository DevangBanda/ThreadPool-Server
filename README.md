# ThreadPool HTTP Server (C++)

A multithreaded HTTP/1.1 server implemented from scratch in modern C++
using POSIX sockets and a bounded thread pool.

This project demonstrates networking fundamentals, concurrency
correctness, resource management, and clean architecture suitable for
backend and systems programming roles.

------------------------------------------------------------------------

## Overview

This server accepts TCP connections, parses HTTP requests manually,
routes requests to handlers, and returns structured HTTP responses. It
uses a bounded thread pool with a producer--consumer queue to control
concurrency and prevent thread explosion.

Design goals:

-   Predictable resource usage
-   Proper synchronization
-   RAII-based socket management
-   Signal-driven graceful shutdown
-   Clear separation of networking, HTTP logic, and concurrency

------------------------------------------------------------------------

## Features

-   TCP server using POSIX sockets
-   Manual HTTP request parsing (request line + headers)
-   Thread pool with bounded blocking queue
-   Basic routing system (`GET` endpoints)
-   Graceful shutdown via `SIGINT`
-   RAII socket wrapper for safe resource cleanup
-   Load testable using `wrk`
-   Modular and extensible architecture

------------------------------------------------------------------------

## Architecture

Client → TCP Listener → ThreadPool → HTTP Parser → Router → HTTP
Response

### Project Structure

    src/
      main.cpp
      core/
        ThreadPool.cpp
        BlockingQueue.h
      net/
        TcpListener.cpp
        Socket.h
      http/
        HttpParser.cpp
        HttpRequest.h
        HttpResponse.cpp
        Router.cpp

-   `core/` → concurrency primitives\
-   `net/` → networking layer\
-   `http/` → HTTP parsing and routing\
-   `main.cpp` → server bootstrap and lifecycle management

------------------------------------------------------------------------

## Build Instructions

### Requirements

-   C++17
-   CMake
-   POSIX-compatible system (Linux/macOS)

### Build

``` bash
mkdir build
cd build
cmake ..
cmake --build . -j
```

### Run

``` bash
./http_server
```

Server runs on:

http://localhost:8080

------------------------------------------------------------------------

## Example Usage

### Health Endpoint

``` bash
curl -i http://localhost:8080/health
```

Response:

    HTTP/1.1 200 OK
    Content-Type: text/plain
    Content-Length: 3

    ok

------------------------------------------------------------------------

### Root Endpoint

``` bash
curl -i http://localhost:8080/
```

Response:

    HTTP/1.1 200 OK
    Content-Type: text/html; charset=utf-8

    <h1>hello from C++</h1>

------------------------------------------------------------------------

## Concurrency Model

-   Fixed-size thread pool
-   Bounded blocking job queue
-   Worker threads handle request lifecycle
-   Main thread handles connection acceptance
-   Graceful shutdown closes listener and joins workers

This prevents:

-   Unbounded thread creation
-   Resource exhaustion under load
-   Race conditions during shutdown

------------------------------------------------------------------------

## Benchmarking

Install wrk:

``` bash
brew install wrk
```

Run load test:

``` bash
wrk -t8 -c200 -d10s http://127.0.0.1:8080/health
```

------------------------------------------------------------------------

## Technical Highlights

-   RAII-based socket wrapper
-   Condition variable--based blocking queue
-   Signal-driven lifecycle management
-   Modular layered architecture
-   Designed for extensibility (timeouts, keep-alive, async logging)

------------------------------------------------------------------------

## Future Improvements

-   HTTP keep-alive support
-   Connection timeouts
-   Async structured logging
-   Metrics endpoint
-   Reverse proxy mode
-   Non-blocking I/O (epoll)
