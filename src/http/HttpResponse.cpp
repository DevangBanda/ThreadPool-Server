#include "HttpResponse.h"
#include <sstream>

std::string HttpResponse::toString() const {
    std::ostringstream oss;
    oss << "HTTP/1.1 " << status << " " << reason << "\r\n";
    for (auto& [k,v] : headers) oss << k << ": " << v << "\r\n";
    oss << "Content-Length: " << body.size() << "\r\n";
    oss << "\r\n";
    oss << body;
    return oss.str();
}