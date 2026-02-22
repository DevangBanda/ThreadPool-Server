#pragma once
#include <string>
#include <unordered_map>

struct HttpRequest {
    std::string method;
    std::string target;   // includes path + query
    std::string path;     // path only
    std::string query;    // raw query
    std::string version;
    std::unordered_map<std::string, std::string> headers;
};