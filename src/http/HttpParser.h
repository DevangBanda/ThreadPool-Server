#pragma once
#include "HttpRequest.h"
#include <string>

class HttpParser {
public:
    // returns true if parsed a complete request (headers end found)
    static bool parseRequest(const std::string& raw, HttpRequest& out);
};