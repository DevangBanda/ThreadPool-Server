#pragma once
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <functional>
#include <string>
#include <unordered_map>

class Router {
public:
    using Handler = std::function<HttpResponse(const HttpRequest&)>;

    void get(const std::string& path, Handler h);
    HttpResponse route(const HttpRequest& req) const;

private:
    std::unordered_map<std::string, Handler> get_;
};