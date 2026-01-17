#include "UserRoutes.h"
#include <nlohmann/json.hpp>
#include <string>
#include "../repository/postgres/PostgresConnection.h"
#include <pqxx/pqxx>

using json = nlohmann::json;

void registerUserRoutes(httplib::Server& server) {

    // GET all users - GET /api/users
    server.Get("/api/users", [](const httplib::Request&, httplib::Response& res) {
        try {
            pqxx::work txn(PostgresConnection::getConnection());
            pqxx::result r = txn.exec("SELECT id, name, email, role FROM users");
            
            json response = json::array();
            for (const auto& row : r) {
                response.push_back(json{
                    {"id", row["id"].as<int>()},
                    {"name", row["name"].as<std::string>()},
                    {"email", row["email"].as<std::string>()},
                    {"role", row["role"].as<std::string>()}
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

    // GET user by ID - GET /api/users/:id
    server.Get(R"(/api/users/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int userId = std::stoi(req.matches[1]);
            
            pqxx::work txn(PostgresConnection::getConnection());
            pqxx::result r = txn.exec_params(
                "SELECT id, name, email, role FROM users WHERE id = $1",
                userId
            );
            
            if (r.empty()) {
                res.set_content(json{{"error", "User not found"}}.dump(), "application/json");
                res.status = 404;
                return;
            }
            
            json response = json{
                {"id", r[0]["id"].as<int>()},
                {"name", r[0]["name"].as<std::string>()},
                {"email", r[0]["email"].as<std::string>()},
                {"role", r[0]["role"].as<std::string>()}
            };
            txn.commit();
            
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
            
            // Validate role - must be ADMIN or USER (uppercase)
            if (role != "ADMIN" && role != "USER") {
                res.set_content(json{{"error", "Role must be 'ADMIN' or 'USER'"}}.dump(), "application/json");
                res.status = 400;
                return;
            }
            
            // Insert into database
            pqxx::work txn(PostgresConnection::getConnection());
            pqxx::result r = txn.exec_params(
                "INSERT INTO users (name, email, role) VALUES ($1, $2, $3) RETURNING id",
                name, email, role
            );
            int userId = r[0][0].as<int>();
            txn.commit();
            
            json response = json{
                {"id", userId},
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
            
            // Check if user exists
            pqxx::work txn(PostgresConnection::getConnection());
            pqxx::result check = txn.exec_params(
                "SELECT id FROM users WHERE id = $1",
                userId
            );
            
            if (check.empty()) {
                res.set_content(json{{"error", "User not found"}}.dump(), "application/json");
                res.status = 404;
                return;
            }
            
            // Prepare update fields
            std::string name = body.value("name", "");
            std::string email = body.value("email", "");
            std::string role = body.value("role", "");
            
            // Validate role if provided - must be ADMIN or USER (uppercase)
            if (!role.empty() && role != "ADMIN" && role != "USER") {
                res.set_content(json{{"error", "Role must be 'ADMIN' or 'USER'"}}.dump(), "application/json");
                res.status = 400;
                return;
            }
            
            // Build dynamic update query
            std::string query = "UPDATE users SET ";
            std::vector<std::string> updates;
            
            if (!name.empty()) {
                updates.push_back("name = '" + name + "'");
            }
            if (!email.empty()) {
                updates.push_back("email = '" + email + "'");
            }
            if (!role.empty()) {
                updates.push_back("role = '" + role + "'");
            }
            
            if (updates.empty()) {
                res.set_content(json{{"error", "No fields to update"}}.dump(), "application/json");
                res.status = 400;
                return;
            }
            
            // Join updates and add WHERE clause
            for (size_t i = 0; i < updates.size(); ++i) {
                if (i > 0) query += ", ";
                query += updates[i];
            }
            query += " WHERE id = " + std::to_string(userId) + " RETURNING id, name, email, role";
            
            // Execute update
            pqxx::result r = txn.exec(query);
            
            if (r.empty()) {
                res.set_content(json{{"error", "Failed to update user"}}.dump(), "application/json");
                res.status = 500;
                return;
            }
            
            txn.commit();
            
            json response = json{
                {"id", r[0]["id"].as<int>()},
                {"name", r[0]["name"].as<std::string>()},
                {"email", r[0]["email"].as<std::string>()},
                {"role", r[0]["role"].as<std::string>()},
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
            
            // Check if user exists
            pqxx::work txn(PostgresConnection::getConnection());
            pqxx::result check = txn.exec_params(
                "SELECT id FROM users WHERE id = $1",
                userId
            );
            
            if (check.empty()) {
                res.set_content(json{{"error", "User not found"}}.dump(), "application/json");
                res.status = 404;
                return;
            }
            
            // Delete user
            pqxx::result r = txn.exec_params(
                "DELETE FROM users WHERE id = $1 RETURNING id",
                userId
            );
            txn.commit();
            
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
