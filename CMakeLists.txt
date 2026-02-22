cmake_minimum_required(VERSION 3.16)
project(http_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(http_server
  src/main.cpp
  src/core/ThreadPool.cpp
  src/net/TcpListener.cpp
  src/http/HttpParser.cpp
  src/http/HttpResponse.cpp
  src/http/Router.cpp
)

target_include_directories(http_server PRIVATE src)