#include "UserRoutes.h"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

void registerUserRoutes(httplib::Server& server) {

    // GET all users - GET /api/users
    server.Get("/api/users", [](const httplib::Request&, httplib::Response& res) {
        try {
            json response = json::array();
            response.push_back(json{
                {"id", 1},
                {"name", "Admin User"},
                {"email", "admin@inventory.com"},
                {"role", "admin"}
            });
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
            res.status = 500;
        }
    });

    // GET user by ID - GET /api/users/:id
    server.Get(R"(/api/users/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int userId = std::stoi(req.matches[1]);
            json response = json{
                {"id", userId},
                {"name", "User Name"},
                {"email", "user@inventory.com"},
                {"role", "user"}
            };
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
            res.status = 500;
        }
    });

    // CREATE user - POST /api/users
    server.Post("/api/users", [](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);
            
            // Validate required fields
            if (!body.contains("name") || !body.contains("email") || !body.contains("role")) {
                res.set_content(json{{"error", "Missing required fields: name, email, role"}}.dump(), "application/json");
                res.status = 400;
                return;
            }
            
            std::string name = body["name"];
            std::string email = body["email"];
            std::string role = body["role"];
            
            // Validate role
            if (role != "admin" && role != "user") {
                res.set_content(json{{"error", "Role must be 'admin' or 'user'"}}.dump(), "application/json");
                res.status = 400;
                return;
            }
            
            json response = json{
                {"id", 1},
                {"name", name},
                {"email", email},
                {"role", role},
                {"status", "created"}
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

    // UPDATE user - PUT /api/users/:id
    server.Put(R"(/api/users/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int userId = std::stoi(req.matches[1]);
            json body = json::parse(req.body);
            
            json response = json{
                {"id", userId},
                {"name", body.value("name", "User Name")},
                {"email", body.value("email", "user@inventory.com")},
                {"role", body.value("role", "user")},
                {"status", "updated"}
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

    // DELETE user - DELETE /api/users/:id
    server.Delete(R"(/api/users/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int userId = std::stoi(req.matches[1]);
            json response = json{
                {"id", userId},
                {"status", "deleted"},
                {"message", "User deleted successfully"}
            };
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
            res.status = 500;
        }
    });
}
