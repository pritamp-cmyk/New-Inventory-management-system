#include "ProductRoutes.h"
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>
#include "../repository/postgres/PostgresConnection.h"
#include "../repository/postgres/ProductRepo.cpp"
#include "../service/implementations/InventoryService.cpp"

using json = nlohmann::json;

void registerProductRoutes(httplib::Server& server) {
    // Create repositories and services
    static ProductRepo productRepo;

    // SEARCH products by name - GET /api/products/search?name=xyz (MUST come before :id pattern)
    server.Get(R"(/api/products/search)", [](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string searchName = req.get_param_value("name");
            if (searchName.empty()) {
                res.set_content(json{{"error", "name parameter required"}}.dump(), "application/json");
                res.status = 400;
                return;
            }
            
            ProductRepo repo;
            auto products = repo.find_by_name(searchName);
            
            json response = json::array();
            for (const auto& product : products) {
                response.push_back(json{
                    {"id", product.get_id()},
                    {"name", product.get_name()},
                    {"description", product.get_description()}
                });
            }
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
            res.status = 500;
        }
    });

    // GET all products
    server.Get("/api/products", [](const httplib::Request&, httplib::Response& res) {
        try {
            ProductRepo repo;
            
            // Query all products from database
            pqxx::work txn(PostgresConnection::getConnection());
            pqxx::result r = txn.exec("SELECT id, name, description FROM products");
            
            json response = json::array();
            for (const auto& row : r) {
                response.push_back(json{
                    {"id", row["id"].as<int>()},
                    {"name", row["name"].as<std::string>()},
                    {"description", row["description"].as<std::string>()}
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

    // GET product by ID - GET /api/products/:id
    server.Get(R"(/api/products/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int productId = std::stoi(req.matches[1]);
            
            ProductRepo repo;
            product p = repo.find_by_id(productId);
            
            json response = json{
                {"id", p.get_id()},
                {"name", p.get_name()},
                {"description", p.get_description()}
            };
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
            res.status = 500;
        }
    });

    // CREATE product - POST /api/products
    server.Post("/api/products", [](const httplib::Request& req, httplib::Response& res) {
        try {
            json body = json::parse(req.body);
            
            // Validate required fields
            if (!body.contains("name") || !body.contains("description") || !body.contains("initial_stock")) {
                res.set_content(json{{"error", "Missing required fields: name, description, initial_stock"}}.dump(), "application/json");
                res.status = 400;
                return;
            }
            
            std::string name = body["name"];
            std::string description = body["description"];
            int initialStock = body["initial_stock"];
            
            // Create product and inventory in database
            ProductRepo productRepo;
            int productId = productRepo.create(name, description);
            
            // TODO: Add inventory after inventory repo is implemented
            
            json response = json{
                {"id", productId},
                {"name", name},
                {"description", description},
                {"stock", initialStock},
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

    // UPDATE product - PUT /api/products/:id
    server.Put(R"(/api/products/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int productId = std::stoi(req.matches[1]);
            json body = json::parse(req.body);
            
            // Validate at least one field is provided
            if (!body.contains("name") && !body.contains("description")) {
                res.set_content(json{{"error", "At least one field (name or description) required"}}.dump(), "application/json");
                res.status = 400;
                return;
            }
            
            std::string name = body.value("name", "");
            std::string description = body.value("description", "");
            
            // Update product in database
            ProductRepo repo;
            repo.update(productId, name, description);
            
            json response = json{
                {"id", productId},
                {"status", "updated"},
                {"name", name},
                {"description", description}
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

    // DELETE product - DELETE /api/products/:id
    server.Delete(R"(/api/products/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int productId = std::stoi(req.matches[1]);
            
            // Delete product from database
            ProductRepo repo;
            repo.remove(productId);
            
            json response = json{
                {"id", productId},
                {"status", "deleted"},
                {"message", "Product deleted successfully"}
            };
            
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
            res.status = 500;
        }
    });

    // TRACK/GET inventory - GET /api/inventory/:product_id
    server.Get(R"(/api/inventory/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int productId = std::stoi(req.matches[1]);
            
            // Query inventory from database
            pqxx::work txn(PostgresConnection::getConnection());
            pqxx::result r = txn.exec_params(
                "SELECT product_id, stock, updated_at FROM inventory WHERE product_id = $1",
                productId
            );
            
            if (r.empty()) {
                res.set_content(json{{"error", "Inventory not found"}}.dump(), "application/json");
                res.status = 404;
                return;
            }
            
            json response = json{
                {"product_id", r[0]["product_id"].as<int>()},
                {"stock", r[0]["stock"].as<int>()},
                {"updated_at", r[0]["updated_at"].as<std::string>()},
                {"status", r[0]["stock"].as<int>() > 0 ? "in_stock" : "out_of_stock"}
            };
            txn.commit();
            
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
            res.status = 500;
        }
    });

    // UPDATE inventory/stock - PUT /api/inventory/:product_id
    server.Put(R"(/api/inventory/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int productId = std::stoi(req.matches[1]);
            json body = json::parse(req.body);
            
            if (!body.contains("stock")) {
                res.set_content(json{{"error", "stock field required"}}.dump(), "application/json");
                res.status = 400;
                return;
            }
            
            int newStock = body["stock"];
            
            // Update inventory in database
            pqxx::work txn(PostgresConnection::getConnection());
            txn.exec_params(
                "UPDATE inventory SET stock=$1, updated_at=CURRENT_TIMESTAMP WHERE product_id=$2",
                newStock, productId
            );
            txn.commit();
            
            json response = json{
                {"product_id", productId},
                {"new_stock", newStock},
                {"status", "updated"},
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
}
