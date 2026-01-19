#include "NotificationController.h"
#include "service/implementations/NotificationService.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

void NotificationController::registerRoutes(httplib::Server& svr) {
    auto notification_service = std::make_shared<NotificationService>();
    
    // POST /api/notifications/subscribe - Subscribe to product notifications
    svr.Post("/api/notifications/subscribe", [notification_service](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            
            if (!body.contains("user_id") || !body.contains("product_id")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
                res.set_content(
                    json({
                        {"status", "error"},
                        {"message", "Missing user_id or product_id"}
                    }).dump(),
                    "application/json"
                );
                res.status = 400;
                return;
            }
            
            int user_id = body["user_id"];
            int product_id = body["product_id"];
            
            bool success = notification_service->subscribeUser(user_id, product_id);
            
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            
            if (success) {
                res.set_content(
                    json({
                        {"status", "success"},
                        {"message", "Successfully subscribed to notifications"},
                        {"user_id", user_id},
                        {"product_id", product_id}
                    }).dump(),
                    "application/json"
                );
                res.status = 200;
            } else {
                res.set_content(
                    json({
                        {"status", "error"},
                        {"message", "Failed to subscribe to notifications"}
                    }).dump(),
                    "application/json"
                );
                res.status = 500;
            }
        } catch (const std::exception& e) {
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            res.set_content(
                json({
                    {"status", "error"},
                    {"message", std::string(e.what())}
                }).dump(),
                "application/json"
            );
            res.status = 500;
        }
    });
    
    // DELETE /api/notifications/:notification_id - Unsubscribe from notifications
    svr.Delete("/api/notifications/(\\d+)", [notification_service](const httplib::Request& req, httplib::Response& res) {
        try {
            int notification_id = std::stoi(req.matches[1]);
            
            bool success = notification_service->unsubscribeUser(notification_id);
            
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            
            if (success) {
                res.set_content(
                    json({
                        {"status", "success"},
                        {"message", "Successfully unsubscribed from notifications"}
                    }).dump(),
                    "application/json"
                );
                res.status = 200;
            } else {
                res.set_content(
                    json({
                        {"status", "error"},
                        {"message", "Failed to unsubscribe from notifications"}
                    }).dump(),
                    "application/json"
                );
                res.status = 500;
            }
        } catch (const std::exception& e) {
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            res.set_content(
                json({
                    {"status", "error"},
                    {"message", std::string(e.what())}
                }).dump(),
                "application/json"
            );
            res.status = 500;
        }
    });
    
    // GET /api/notifications/:user_id - Get user's notifications
    svr.Get("/api/notifications/user/(\\d+)", [notification_service](const httplib::Request& req, httplib::Response& res) {
        try {
            int user_id = std::stoi(req.matches[1]);
            
            auto notifications = notification_service->getUserNotifications(user_id);
            
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            
            json response = json::array();
            for (const auto& notif : notifications) {
                response.push_back({
                    {"id", notif.id},
                    {"product_id", notif.product_id},
                    {"user_id", notif.user_id},
                    {"type", notif.type_str},
                    {"is_sent", notif.is_sent},
                    {"created_at", notif.created_at},
                    {"sent_at", notif.sent_at}
                });
            }
            
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            res.set_content(
                json({
                    {"status", "error"},
                    {"message", std::string(e.what())}
                }).dump(),
                "application/json"
            );
            res.status = 500;
        }
    });
    
    // GET /api/notifications/product/:product_id - Get product subscribers
    svr.Get("/api/notifications/product/(\\d+)", [notification_service](const httplib::Request& req, httplib::Response& res) {
        try {
            int product_id = std::stoi(req.matches[1]);
            
            auto subscribers = notification_service->getProductSubscribers(product_id);
            
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            
            json response = json::array();
            for (const auto& sub : subscribers) {
                response.push_back({
                    {"id", sub.id},
                    {"product_id", sub.product_id},
                    {"user_id", sub.user_id},
                    {"type", sub.type_str},
                    {"is_sent", sub.is_sent},
                    {"subscribed_at", sub.created_at}
                });
            }
            
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            res.set_content(
                json({
                    {"status", "error"},
                    {"message", std::string(e.what())}
                }).dump(),
                "application/json"
            );
            res.status = 500;
        }
    });
    
    // GET /api/notifications/logs/:user_id - Get notification logs for user
    svr.Get("/api/notifications/logs/user/(\\d+)", [notification_service](const httplib::Request& req, httplib::Response& res) {
        try {
            int user_id = std::stoi(req.matches[1]);
            
            auto logs = notification_service->getNotificationLogs(user_id);
            
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            
            json response = json::array();
            for (const auto& log : logs) {
                response.push_back({
                    {"id", log.id},
                    {"notification_id", log.notification_id},
                    {"user_id", log.user_id},
                    {"product_id", log.product_id},
                    {"type", log.notification_type},
                    {"message", log.message},
                    {"status", log.status},
                    {"retry_count", log.retry_count},
                    {"max_retries", log.max_retries},
                    {"error_message", log.error_message},
                    {"sent_at", log.sent_at},
                    {"created_at", log.created_at}
                });
            }
            
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            res.set_content(
                json({
                    {"status", "error"},
                    {"message", std::string(e.what())}
                }).dump(),
                "application/json"
            );
            res.status = 500;
        }
    });
    
    // GET /api/notifications/logs/failed - Get all failed notifications
    svr.Get("/api/notifications/logs/status/failed", [notification_service](const httplib::Request& req, httplib::Response& res) {
        try {
            auto logs = notification_service->getFailedNotifications();
            
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            
            json response = json::array();
            for (const auto& log : logs) {
                response.push_back({
                    {"id", log.id},
                    {"notification_id", log.notification_id},
                    {"user_id", log.user_id},
                    {"product_id", log.product_id},
                    {"type", log.notification_type},
                    {"message", log.message},
                    {"status", log.status},
                    {"retry_count", log.retry_count},
                    {"max_retries", log.max_retries},
                    {"error_message", log.error_message},
                    {"created_at", log.created_at}
                });
            }
            
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            res.set_content(
                json({
                    {"status", "error"},
                    {"message", std::string(e.what())}
                }).dump(),
                "application/json"
            );
            res.status = 500;
        }
    });
    
    // POST /api/notifications/logs/:log_id/retry - Retry failed notification
    svr.Post("/api/notifications/logs/(\\d+)/retry", [notification_service](const httplib::Request& req, httplib::Response& res) {
        try {
            int log_id = std::stoi(req.matches[1]);
            
            bool success = notification_service->retryFailedNotification(log_id);
            
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            
            if (success) {
                res.set_content(
                    json({
                        {"status", "success"},
                        {"message", "Notification retry initiated"}
                    }).dump(),
                    "application/json"
                );
                res.status = 200;
            } else {
                res.set_content(
                    json({
                        {"status", "error"},
                        {"message", "Failed to retry notification"}
                    }).dump(),
                    "application/json"
                );
                res.status = 500;
            }
        } catch (const std::exception& e) {
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            res.set_content(
                json({
                    {"status", "error"},
                    {"message", std::string(e.what())}
                }).dump(),
                "application/json"
            );
            res.status = 500;
        }
    });
    
    // PUT /api/notifications/preferences/:user_id - Update notification preferences
    svr.Put("/api/notifications/preferences/(\\d+)", [notification_service](const httplib::Request& req, httplib::Response& res) {
        try {
            int user_id = std::stoi(req.matches[1]);
            auto body = json::parse(req.body);
            
            bool email = body.value("email_enabled", true);
            bool push = body.value("push_enabled", false);
            bool sms = body.value("sms_enabled", false);
            bool in_app = body.value("in_app_enabled", true);
            
            bool success = notification_service->updateUserPreferences(user_id, email, push, sms, in_app);
            
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            
            if (success) {
                res.set_content(
                    json({
                        {"status", "success"},
                        {"message", "Notification preferences updated"}
                    }).dump(),
                    "application/json"
                );
                res.status = 200;
            } else {
                res.set_content(
                    json({
                        {"status", "error"},
                        {"message", "Failed to update preferences"}
                    }).dump(),
                    "application/json"
                );
                res.status = 500;
            }
        } catch (const std::exception& e) {
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            res.set_content(
                json({
                    {"status", "error"},
                    {"message", std::string(e.what())}
                }).dump(),
                "application/json"
            );
            res.status = 500;
        }
    });
    
    // GET /api/notifications/preferences/:user_id - Get notification preferences
    svr.Get("/api/notifications/preferences/(\\d+)", [notification_service](const httplib::Request& req, httplib::Response& res) {
        try {
            int user_id = std::stoi(req.matches[1]);
            
            auto prefs = notification_service->getUserPreferences(user_id);
            
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            
            res.set_content(
                json({
                    {"id", prefs.id},
                    {"user_id", prefs.user_id},
                    {"email_enabled", prefs.email_enabled},
                    {"push_enabled", prefs.push_enabled},
                    {"sms_enabled", prefs.sms_enabled},
                    {"in_app_enabled", prefs.in_app_enabled},
                    {"created_at", prefs.created_at},
                    {"updated_at", prefs.updated_at}
                }).dump(),
                "application/json"
            );
            res.status = 200;
        } catch (const std::exception& e) {
            if (!res.has_header("Access-Control-Allow-Origin")) {
                res.set_header("Access-Control-Allow-Origin", "http://localhost:3001");
            }
            res.set_content(
                json({
                    {"status", "error"},
                    {"message", std::string(e.what())}
                }).dump(),
                "application/json"
            );
            res.status = 500;
        }
    });
}
