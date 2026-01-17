#include <iostream>
#include "external/httplib.h"

#include "../src/controller/ProductRoutes.h"
#include "../src/controller/UserRoutes.h"
#include "../src/controller/SubscriptionRoutes.h"

int main() {
    httplib::Server server;

    // Handle CORS preflight requests
    server.Options(R"(/api/.*)", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        res.set_header("Access-Control-Max-Age", "3600");
        res.status = 204;
    });

    // Add CORS headers to all responses using post_routing_handler
    server.set_post_routing_handler([](const httplib::Request&, httplib::Response& res) {
        // Only add CORS header if not already set
        if (!res.has_header("Access-Control-Allow-Origin")) {
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        }
        return httplib::Server::HandlerResponse::Handled;
    });

    registerProductRoutes(server);
    registerUserRoutes(server);
    registerSubscriptionRoutes(server);

    std::cout << "Server running on http://localhost:8080\n";
    std::cout << "CORS enabled for all origins\n";
    server.listen("0.0.0.0", 8080);
}
