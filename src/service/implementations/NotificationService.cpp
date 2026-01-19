#include "NotificationService.h"
#include "../repository/postgres/NotificationRepo.h"
#include "../repository/postgres/PostgresConnection.h"
#include <iostream>
#include <sstream>

NotificationService::NotificationService() 
    : notification_repo(std::make_unique<NotificationRepo>()) {
}

bool NotificationService::subscribeUser(int user_id, int product_id) {
    try {
        // Verify user exists
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        pqxx::result user_check = txn.exec_params(
            "SELECT id FROM users WHERE id = $1",
            user_id
        );
        
        if (user_check.empty()) {
            std::cerr << "User " << user_id << " not found" << std::endl;
            return false;
        }
        
        pqxx::result product_check = txn.exec_params(
            "SELECT id FROM products WHERE id = $1",
            product_id
        );
        
        if (product_check.empty()) {
            std::cerr << " Product " << product_id << " not found" << std::endl;
            return false;
        }
        
        txn.commit();
        
        // Subscribe to notifications
        return notification_repo->subscribeToNotification(user_id, product_id, "restocked");
    } catch (const std::exception& e) {
        std::cerr << "Error subscribing user: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationService::unsubscribeUser(int notification_id) {
    return notification_repo->unsubscribeFromNotification(notification_id);
}

std::vector<domain::Notification> NotificationService::getUserNotifications(int user_id) {
    return notification_repo->getUserNotifications(user_id);
}

std::vector<domain::Notification> NotificationService::getProductSubscribers(int product_id) {
    return notification_repo->getProductSubscribers(product_id);
}

bool NotificationService::sendRestockNotifications(int product_id, int stock) {
    try {
        if (stock <= 0) {
            std::cout << "⚠️  Product " << product_id << " is out of stock, not sending notifications" << std::endl;
            return true;
        }
        
        // Get all subscribers for this product who haven't been notified yet
        std::vector<domain::Notification> subscribers = notification_repo->getProductSubscribers(product_id);
        
        if (subscribers.empty()) {
            std::cout << "ℹ️  No subscribers for product " << product_id << std::endl;
            return true;
        }
        
        std::cout << "📢 Sending restock notifications to " << subscribers.size() << " subscribers of product " << product_id << std::endl;
        
        auto& conn = PostgresConnection::getConnection();
        
        for (const auto& sub : subscribers) {
            if (sub.is_sent) {
                continue; // Skip already notified subscribers
            }
            
            std::string message = "Product back in stock! Check it out now.";
            
            if (!sendNotification(sub.id, message)) {
                std::cerr << "Failed to send notification " << sub.id << " to user " << sub.user_id << std::endl;
            }
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error sending restock notifications: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationService::sendNotification(int notification_id, const std::string& message) {
    try {
        // Get notification details
        domain::Notification notif = notification_repo->getNotificationById(notification_id);
        if (notif.id == 0) {
            std::cerr << "Notification " << notification_id << " not found" << std::endl;
            return false;
        }
        
        // Get user preferences
        domain::NotificationPreference prefs = notification_repo->getNotificationPreference(notif.user_id);
        
        // Create notification log entry
        domain::NotificationLog log;
        log.notification_id = notification_id;
        log.user_id = notif.user_id;
        log.product_id = notif.product_id;
        log.notification_type = notif.type_str;
        log.message = message;
        log.status = "pending";
        log.retry_count = 0;
        log.max_retries = 3;
        
        int log_id = notification_repo->createNotificationLog(log);
        if (log_id < 0) {
            std::cerr << "Failed to create notification log" << std::endl;
            return false;
        }
        
        // Simulate notification sending (in real app, send email, push, etc.)
        bool send_success = simulateSendNotification(notif.user_id, notif.product_id, message, prefs);
        
        if (send_success) {
            // Mark notification as sent
            notification_repo->markNotificationAsSent(notification_id);
            notification_repo->updateNotificationLogStatus(log_id, "sent");
            
            std::cout << "Notification sent to user " << notif.user_id 
                     << " for product " << notif.product_id << std::endl;
            return true;
        } else {
            // Mark as failed in log
            notification_repo->updateNotificationLogStatus(log_id, "failed", "Failed to send notification");
            
            std::cerr << " Failed to send notification to user " << notif.user_id << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in sendNotification: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationService::simulateSendNotification(int user_id, int product_id, 
                                                   const std::string& message,
                                                   const domain::NotificationPreference& prefs) {
    try {
        std::ostringstream ss;
        ss << "📧 [NOTIFICATION LOG] User " << user_id 
           << " - Product " << product_id 
           << " - Message: " << message << std::endl;
        ss << "   ├─ Email: " << (prefs.email_enabled ? "enabled" : "disabled") << std::endl;
        ss << "   ├─ Push: " << (prefs.push_enabled ? "enabled" : "disabled") << std::endl;
        ss << "   ├─ SMS: " << (prefs.sms_enabled ? "enabled" : "disabled") << std::endl;
        ss << "   └─ In-App: " << (prefs.in_app_enabled ? "enabled" : "disabled") << std::endl;
        
        std::cout << ss.str();
        
        // In a real implementation:
        // - Send email via SMTP if email_enabled
        // - Send push via Firebase/APNs if push_enabled
        // - Send SMS via Twilio if sms_enabled
        // - Create in-app notification if in_app_enabled
        
        return true; // Simulate successful send
    } catch (const std::exception& e) {
        std::cerr << "Error simulating notification: " << e.what() << std::endl;
        return false;
    }
}

std::vector<domain::NotificationLog> NotificationService::getNotificationLogs(int user_id) {
    return notification_repo->getNotificationLogs(user_id);
}

std::vector<domain::NotificationLog> NotificationService::getFailedNotifications() {
    return notification_repo->getFailedNotifications();
}

bool NotificationService::retryFailedNotification(int log_id) {
    try {
        std::vector<domain::NotificationLog> logs = notification_repo->getFailedNotifications();
        
        domain::NotificationLog target_log;
        for (const auto& log : logs) {
            if (log.id == log_id) {
                target_log = log;
                break;
            }
        }
        
        if (target_log.id == 0) {
            std::cerr << "Failed notification " << log_id << " not found or already recovered" << std::endl;
            return false;
        }
        
        if (target_log.retry_count >= target_log.max_retries) {
            std::cerr << "Max retries reached for notification " << log_id << std::endl;
            return false;
        }
        
        // Increment retry count
        notification_repo->incrementRetryCount(log_id);
        
        // Try sending again
        std::string message = target_log.message;
        if (sendNotification(target_log.notification_id, message)) {
            notification_repo->updateNotificationLogStatus(log_id, "retried");
            std::cout << "Notification " << log_id << " retried successfully" << std::endl;
            return true;
        } else {
            std::cerr << "Retry failed for notification " << log_id << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error retrying notification: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationService::updateUserPreferences(int user_id, bool email, bool push, bool sms, bool in_app) {
    try {
        domain::NotificationPreference pref = notification_repo->getNotificationPreference(user_id);
        pref.email_enabled = email;
        pref.push_enabled = push;
        pref.sms_enabled = sms;
        pref.in_app_enabled = in_app;
        
        return notification_repo->updateNotificationPreference(pref);
    } catch (const std::exception& e) {
        std::cerr << " Error updating user preferences: " << e.what() << std::endl;
        return false;
    }
}

domain::NotificationPreference NotificationService::getUserPreferences(int user_id) {
    return notification_repo->getNotificationPreference(user_id);
}
