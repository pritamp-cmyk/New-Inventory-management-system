#include "SubscriptionRoutes.h"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

void registerSubscriptionRoutes(httplib::Server& server) {

    // GET all subscriptions - GET /api/subscriptions
    server.Get("/api/subscriptions", [](const httplib::Request&, httplib::Response& res) {
        try {
            json response = json::array();
            response.push_back(json{
                {"id", 1},
                {"user_id", 1},
                {"product_id", 1},
                {"active", true},
                {"created_at", "2026-01-16T12:00:00Z"}
            });
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
            res.status = 500;
        }
    });

    // GET user's subscriptions - GET /api/users/:user_id/subscriptions
    server.Get(R"(/api/users/(\d+)/subscriptions)", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int userId = std::stoi(req.matches[1]);
            json response = json::array();
            response.push_back(json{
                {"id", 1},
                {"user_id", userId},
                {"product_id", 1},
                {"active", true},
                {"created_at", "2026-01-16T12:00:00Z"}
            });
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
            res.status = 500;
        }
    });

    // CREATE subscription - POST /api/subscriptions
    server.Post("/api/subscriptions", [](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);
            
            // Validate required fields
            if (!body.contains("user_id") || !body.contains("product_id")) {
                res.set_content(json{{"error", "Missing required fields: user_id, product_id"}}.dump(), "application/json");
                res.status = 400;
                return;
            }
            
            int userId = body["user_id"];
            int productId = body["product_id"];
            
            json response = json{
                {"id", 1},
                {"user_id", userId},
                {"product_id", productId},
                {"active", true},
                {"status", "subscribed"},
                {"created_at", "2026-01-16T12:00:00Z"}
            };
            res.set_content(response.dump(), "application/json");
            res.status = 201;
        } catch (const json::exception& e) {
            res.set_content(json{{"error", "Invalid JSON"}}.dump(), "application/json");
            res.status = 400;
        } catch (const std::exception& e) {
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
            res.status = 500;
        }
    });

    // GET subscription by ID - GET /api/subscriptions/:id
    server.Get(R"(/api/subscriptions/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int subscriptionId = std::stoi(req.matches[1]);
            json response = json{
                {"id", subscriptionId},
                {"user_id", 1},
                {"product_id", 1},
                {"active", true},
                {"created_at", "2026-01-16T12:00:00Z"}
            };
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
            res.status = 500;
        }
    });

    // UPDATE subscription status - PUT /api/subscriptions/:id
    server.Put(R"(/api/subscriptions/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int subscriptionId = std::stoi(req.matches[1]);
            json body = json::parse(req.body);
            
            bool active = body.value("active", true);
            
            json response = json{
                {"id", subscriptionId},
                {"active", active},
                {"status", active ? "activated" : "deactivated"},
                {"updated_at", "2026-01-16T12:00:00Z"}
            };
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const json::exception& e) {
            res.set_content(json{{"error", "Invalid JSON"}}.dump(), "application/json");
            res.status = 400;
        } catch (const std::exception& e) {
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
            res.status = 500;
        }
    });

    // UNSUBSCRIBE / DELETE subscription - DELETE /api/subscriptions/:id
    server.Delete(R"(/api/subscriptions/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int subscriptionId = std::stoi(req.matches[1]);
            json response = json{
                {"id", subscriptionId},
                {"status", "unsubscribed"},
                {"message", "Subscription deleted successfully"}
            };
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
            res.status = 500;
        }
    });
}
