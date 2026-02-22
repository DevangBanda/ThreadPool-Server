#pragma once
#include <string>
#include <unordered_map>

struct HttpResponse {
    int status = 200;
    std::string reason = "OK";
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    std::string toString() const;
};