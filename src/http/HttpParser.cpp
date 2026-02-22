#include "HttpParser.h"
#include <sstream>

static inline std::string trim(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    size_t e = s.find_last_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    return s.substr(b, e - b + 1);
}

bool HttpParser::parseRequest(const std::string& raw, HttpRequest& out) {
    // Must contain header terminator
    auto end = raw.find("\r\n\r\n");
    if (end == std::string::npos) return false;

    std::istringstream iss(raw.substr(0, end));
    std::string line;

    if (!std::getline(iss, line)) return false;
    if (!line.empty() && line.back() == '\r') line.pop_back();

    std::istringstream rl(line);
    rl >> out.method >> out.target >> out.version;

    // split target into path/query
    auto qpos = out.target.find('?');
    out.path = (qpos == std::string::npos) ? out.target : out.target.substr(0, qpos);
    out.query = (qpos == std::string::npos) ? "" : out.target.substr(qpos + 1);

    while (std::getline(iss, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) break;
        auto colon = line.find(':');
        if (colon == std::string::npos) continue;
        auto k = trim(line.substr(0, colon));
        auto v = trim(line.substr(colon + 1));
        out.headers[k] = v;
    }
    return true;
}