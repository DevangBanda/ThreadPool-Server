#include "Router.h"

void Router::get(const std::string& path, Handler h) { get_[path] = std::move(h); }

HttpResponse Router::route(const HttpRequest& req) const {
    if (req.method == "GET") {
        auto it = get_.find(req.path);
        if (it != get_.end()) return it->second(req);
    }
    HttpResponse r;
    r.status = 404; r.reason = "Not Found";
    r.headers["Content-Type"] = "text/plain";
    r.body = "404 Not Found\n";
    return r;
}