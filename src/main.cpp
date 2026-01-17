#include <iostream>
#include "external/httplib.h"

#include "../src/controller/ProductRoutes.h"
#include "../src/controller/UserRoutes.h"
#include "../src/controller/SubscriptionRoutes.h"

int main() {
    httplib::Server server;

    registerProductRoutes(server);
    registerUserRoutes(server);
    registerSubscriptionRoutes(server);

    std::cout << "Server running on http://localhost:8080\n";
    server.listen("0.0.0.0", 8080);
}
