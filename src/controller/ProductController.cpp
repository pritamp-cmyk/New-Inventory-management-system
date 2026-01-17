#include "ProductRoutes.h"
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void registerProductRoutes(httplib::Server& server) {

    // GET all products
    server.Get("/api/products", [](const httplib::Request&, httplib::Response& res) {
        try {
            json response = json::array();
            response.push_back(json{
                {"id", 1},
                {"name", "Product 1"},
                {"description", "Sample product"},
                {"price", 99.99}
            });
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
            // TODO: Fetch from service when DB is ready
            json response = json{
                {"id", productId},
                {"name", "Product"},
                {"description", "Sample product"},
                {"price", 99.99}
            };
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            res.set_content(json{{"error", e.what()}}.dump(), "application/json");
            res.status = 500;
        }
    });

    // SEARCH products by name - GET /api/products/search?name=xyz
    server.Get(R"(/api/products/search)", [](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string searchName = req.get_param_value("name");
            if (searchName.empty()) {
                res.set_content(json{{"error", "name parameter required"}}.dump(), "application/json");
                res.status = 400;
                return;
            }
            
            // TODO: Search using service when DB is ready
            json response = json::array();
            response.push_back(json{
                {"id", 1},
                {"name", searchName},
                {"description", "Matching product"},
                {"price", 99.99}
            });
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
            
            // TODO: Call inventoryService.createProduct(name, description, initialStock);
            json response = json{
                {"id", 1},
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
            
            // TODO: Call inventoryService.updateProduct(productId, name, description);
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
            
            // TODO: Call inventoryService.deleteProduct(productId);
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
            
            // TODO: Fetch inventory from service
            json response = json{
                {"product_id", productId},
                {"stock", 100},
                {"updated_at", "2026-01-16T12:00:00Z"},
                {"status", "in_stock"}
            };
            
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
            
            // TODO: Call inventoryService.updateStock(productId, newStock);
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
