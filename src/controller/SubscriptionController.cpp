#include "SubscriptionRoutes.h"
#include <nlohmann/json.hpp>
#include <string>
#include "../repository/postgres/PostgresConnection.h"
#include <pqxx/pqxx>

using json = nlohmann::json;

void registerSubscriptionRoutes(httplib::Server& server) {

    // GET all subscriptions - GET /api/subscriptions
    server.Get("/api/subscriptions", [](const httplib::Request&, httplib::Response& res) {
        try {
            pqxx::work txn(PostgresConnection::getConnection());
            pqxx::result r = txn.exec("SELECT id, user_id, product_id, active, created_at FROM subscriptions");
            
            json response = json::array();
            for (const auto& row : r) {
                response.push_back(json{
                    {"id", row["id"].as<int>()},
                    {"user_id", row["user_id"].as<int>()},
                    {"product_id", row["product_id"].as<int>()},
                    {"active", row["active"].as<bool>()},
                    {"created_at", row["created_at"].as<std::string>()}
                });
            }
            txn.commit();
            
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
            
            pqxx::work txn(PostgresConnection::getConnection());
            pqxx::result r = txn.exec_params(
                "SELECT id, user_id, product_id, active, created_at FROM subscriptions WHERE user_id = $1",
                userId
            );
            
            json response = json::array();
            for (const auto& row : r) {
                response.push_back(json{
                    {"id", row["id"].as<int>()},
                    {"user_id", row["user_id"].as<int>()},
                    {"product_id", row["product_id"].as<int>()},
                    {"active", row["active"].as<bool>()},
                    {"created_at", row["created_at"].as<std::string>()}
                });
            }
            txn.commit();
            
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
            
            pqxx::work txn(PostgresConnection::getConnection());
            pqxx::result r = txn.exec_params(
                "INSERT INTO subscriptions (user_id, product_id, active) VALUES ($1, $2, true) RETURNING id, created_at",
                userId, productId
            );
            int subscriptionId = r[0]["id"].as<int>();
            std::string createdAt = r[0]["created_at"].as<std::string>();
            txn.commit();
            
            json response = json{
                {"id", subscriptionId},
                {"user_id", userId},
                {"product_id", productId},
                {"active", true},
                {"status", "subscribed"},
                {"created_at", createdAt}
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
            
            pqxx::work txn(PostgresConnection::getConnection());
            pqxx::result r = txn.exec_params(
                "SELECT id, user_id, product_id, active, created_at FROM subscriptions WHERE id = $1",
                subscriptionId
            );
            
            if (r.empty()) {
                res.set_content(json{{"error", "Subscription not found"}}.dump(), "application/json");
                res.status = 404;
                return;
            }
            
            json response = json{
                {"id", r[0]["id"].as<int>()},
                {"user_id", r[0]["user_id"].as<int>()},
                {"product_id", r[0]["product_id"].as<int>()},
                {"active", r[0]["active"].as<bool>()},
                {"created_at", r[0]["created_at"].as<std::string>()}
            };
            txn.commit();
            
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
            
            // Check if subscription exists
            pqxx::work txn(PostgresConnection::getConnection());
            pqxx::result check = txn.exec_params(
                "SELECT id FROM subscriptions WHERE id = $1",
                subscriptionId
            );
            
            if (check.empty()) {
                res.set_content(json{{"error", "Subscription not found"}}.dump(), "application/json");
                res.status = 404;
                return;
            }
            
            bool active = body.value("active", true);
            
            // Update subscription status
            pqxx::result r = txn.exec_params(
                "UPDATE subscriptions SET active = $1 WHERE id = $2 RETURNING id, user_id, product_id, active, created_at",
                active, subscriptionId
            );
            txn.commit();
            
            json response = json{
                {"id", r[0]["id"].as<int>()},
                {"user_id", r[0]["user_id"].as<int>()},
                {"product_id", r[0]["product_id"].as<int>()},
                {"active", r[0]["active"].as<bool>()},
                {"status", active ? "activated" : "deactivated"},
                {"updated_at", r[0]["created_at"].as<std::string>()}
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
            
            // Check if subscription exists
            pqxx::work txn(PostgresConnection::getConnection());
            pqxx::result check = txn.exec_params(
                "SELECT id FROM subscriptions WHERE id = $1",
                subscriptionId
            );
            
            if (check.empty()) {
                res.set_content(json{{"error", "Subscription not found"}}.dump(), "application/json");
                res.status = 404;
                return;
            }
            
            // Delete subscription
            pqxx::result r = txn.exec_params(
                "DELETE FROM subscriptions WHERE id = $1 RETURNING id",
                subscriptionId
            );
            txn.commit();
            
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
