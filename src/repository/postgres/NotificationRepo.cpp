#include "NotificationRepo.h"
#include "../postgres/PostgresConnection.h"
#include <iostream>

NotificationRepo::NotificationRepo() {}

bool NotificationRepo::subscribeToNotification(int user_id, int product_id, const std::string& type) {
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        // Check if already subscribed
        pqxx::result r = txn.exec_params(
            "SELECT id FROM product_notifications WHERE user_id = $1 AND product_id = $2 AND notification_type = $3",
            user_id, product_id, type
        );
        
        if (!r.empty()) {
            // Already subscribed, just reset is_sent to false
            txn.exec_params(
                "UPDATE product_notifications SET is_sent = FALSE, updated_at = CURRENT_TIMESTAMP "
                "WHERE user_id = $1 AND product_id = $2 AND notification_type = $3",
                user_id, product_id, type
            );
        } else {
            // New subscription
            txn.exec_params(
                "INSERT INTO product_notifications (product_id, user_id, notification_type, is_sent, created_at, updated_at) "
                "VALUES ($1, $2, $3, FALSE, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)",
                product_id, user_id, type
            );
        }
        
        txn.commit();
        std::cout << "✅ User " << user_id << " subscribed to product " << product_id << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error subscribing to notification: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationRepo::unsubscribeFromNotification(int notification_id) {
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        pqxx::result r = txn.exec_params(
            "DELETE FROM product_notifications WHERE id = $1",
            notification_id
        );
        
        txn.commit();
        std::cout << "✅ Notification " << notification_id << " removed" << std::endl;
        return r.affected_rows() > 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error unsubscribing from notification: " << e.what() << std::endl;
        return false;
    }
}

std::vector<domain::Notification> NotificationRepo::getUserNotifications(int user_id) {
    std::vector<domain::Notification> notifications;
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        pqxx::result r = txn.exec_params(
            "SELECT id, product_id, user_id, notification_type, is_sent, created_at, updated_at, sent_at "
            "FROM product_notifications WHERE user_id = $1 ORDER BY created_at DESC",
            user_id
        );
        
        for (auto row : r) {
            domain::Notification notif;
            notif.id = row["id"].as<int>();
            notif.product_id = row["product_id"].as<int>();
            notif.user_id = row["user_id"].as<int>();
            notif.type_str = row["notification_type"].as<std::string>();
            notif.is_sent = row["is_sent"].as<bool>();
            notif.created_at = row["created_at"].as<std::string>();
            notif.updated_at = row["updated_at"].as<std::string>();
            
            if (!row["sent_at"].is_null()) {
                notif.sent_at = row["sent_at"].as<std::string>();
            }
            
            notifications.push_back(notif);
        }
        
        std::cout << "✅ Retrieved " << notifications.size() << " notifications for user " << user_id << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error getting user notifications: " << e.what() << std::endl;
    }
    
    return notifications;
}

std::vector<domain::Notification> NotificationRepo::getProductSubscribers(int product_id) {
    std::vector<domain::Notification> notifications;
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        pqxx::result r = txn.exec_params(
            "SELECT id, product_id, user_id, notification_type, is_sent, created_at, updated_at "
            "FROM product_notifications WHERE product_id = $1 ORDER BY created_at DESC",
            product_id
        );
        
        for (auto row : r) {
            domain::Notification notif;
            notif.id = row["id"].as<int>();
            notif.product_id = row["product_id"].as<int>();
            notif.user_id = row["user_id"].as<int>();
            notif.type_str = row["notification_type"].as<std::string>();
            notif.is_sent = row["is_sent"].as<bool>();
            notif.created_at = row["created_at"].as<std::string>();
            notif.updated_at = row["updated_at"].as<std::string>();
            
            notifications.push_back(notif);
        }
        
        std::cout << "✅ Retrieved " << notifications.size() << " subscribers for product " << product_id << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error getting product subscribers: " << e.what() << std::endl;
    }
    
    return notifications;
}

domain::Notification NotificationRepo::getNotificationById(int notification_id) {
    domain::Notification notif;
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        pqxx::result r = txn.exec_params(
            "SELECT id, product_id, user_id, notification_type, is_sent, created_at, updated_at, sent_at "
            "FROM product_notifications WHERE id = $1",
            notification_id
        );
        
        if (!r.empty()) {
            auto row = r[0];
            notif.id = row["id"].as<int>();
            notif.product_id = row["product_id"].as<int>();
            notif.user_id = row["user_id"].as<int>();
            notif.type_str = row["notification_type"].as<std::string>();
            notif.is_sent = row["is_sent"].as<bool>();
            notif.created_at = row["created_at"].as<std::string>();
            notif.updated_at = row["updated_at"].as<std::string>();
            
            if (!row["sent_at"].is_null()) {
                notif.sent_at = row["sent_at"].as<std::string>();
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error getting notification by id: " << e.what() << std::endl;
    }
    
    return notif;
}

bool NotificationRepo::isUserSubscribed(int user_id, int product_id) {
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        pqxx::result r = txn.exec_params(
            "SELECT id FROM product_notifications WHERE user_id = $1 AND product_id = $2 AND is_sent = FALSE",
            user_id, product_id
        );
        
        return !r.empty();
    } catch (const std::exception& e) {
        std::cerr << "❌ Error checking subscription: " << e.what() << std::endl;
        return false;
    }
}

int NotificationRepo::getSubscriptionId(int user_id, int product_id) {
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        pqxx::result r = txn.exec_params(
            "SELECT id FROM product_notifications WHERE user_id = $1 AND product_id = $2",
            user_id, product_id
        );
        
        if (!r.empty()) {
            return r[0]["id"].as<int>();
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error getting subscription id: " << e.what() << std::endl;
    }
    
    return -1;
}

bool NotificationRepo::markNotificationAsSent(int notification_id) {
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        txn.exec_params(
            "UPDATE product_notifications SET is_sent = TRUE, sent_at = CURRENT_TIMESTAMP, updated_at = CURRENT_TIMESTAMP "
            "WHERE id = $1",
            notification_id
        );
        
        txn.commit();
        std::cout << "✅ Notification " << notification_id << " marked as sent" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error marking notification as sent: " << e.what() << std::endl;
        return false;
    }
}

int NotificationRepo::createNotificationLog(const domain::NotificationLog& log) {
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        pqxx::result r = txn.exec_params(
            "INSERT INTO notification_logs (notification_id, user_id, product_id, notification_type, message, status, retry_count, max_retries, error_message, created_at, updated_at) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP) "
            "RETURNING id",
            log.notification_id, log.user_id, log.product_id, log.notification_type,
            log.message, log.status, log.retry_count, log.max_retries, log.error_message
        );
        
        int log_id = r[0]["id"].as<int>();
        txn.commit();
        std::cout << "✅ Notification log " << log_id << " created" << std::endl;
        return log_id;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error creating notification log: " << e.what() << std::endl;
        return -1;
    }
}

bool NotificationRepo::updateNotificationLogStatus(int log_id, const std::string& status, const std::string& message) {
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        if (message.empty()) {
            txn.exec_params(
                "UPDATE notification_logs SET status = $1, updated_at = CURRENT_TIMESTAMP WHERE id = $2",
                status, log_id
            );
        } else {
            txn.exec_params(
                "UPDATE notification_logs SET status = $1, error_message = $2, updated_at = CURRENT_TIMESTAMP WHERE id = $3",
                status, message, log_id
            );
        }
        
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error updating notification log status: " << e.what() << std::endl;
        return false;
    }
}

std::vector<domain::NotificationLog> NotificationRepo::getNotificationLogs(int user_id, const std::string& status) {
    std::vector<domain::NotificationLog> logs;
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        pqxx::result r;
        if (status.empty()) {
            r = txn.exec_params(
                "SELECT id, notification_id, user_id, product_id, notification_type, message, status, retry_count, max_retries, error_message, sent_at, created_at, updated_at "
                "FROM notification_logs WHERE user_id = $1 ORDER BY created_at DESC",
                user_id
            );
        } else {
            r = txn.exec_params(
                "SELECT id, notification_id, user_id, product_id, notification_type, message, status, retry_count, max_retries, error_message, sent_at, created_at, updated_at "
                "FROM notification_logs WHERE user_id = $1 AND status = $2 ORDER BY created_at DESC",
                user_id, status
            );
        }
        
        for (auto row : r) {
            domain::NotificationLog log;
            log.id = row["id"].as<int>();
            if (!row["notification_id"].is_null()) {
                log.notification_id = row["notification_id"].as<int>();
            }
            log.user_id = row["user_id"].as<int>();
            log.product_id = row["product_id"].as<int>();
            log.notification_type = row["notification_type"].as<std::string>();
            log.message = row["message"].as<std::string>();
            log.status = row["status"].as<std::string>();
            log.retry_count = row["retry_count"].as<int>();
            log.max_retries = row["max_retries"].as<int>();
            if (!row["error_message"].is_null()) {
                log.error_message = row["error_message"].as<std::string>();
            }
            if (!row["sent_at"].is_null()) {
                log.sent_at = row["sent_at"].as<std::string>();
            }
            log.created_at = row["created_at"].as<std::string>();
            log.updated_at = row["updated_at"].as<std::string>();
            
            logs.push_back(log);
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error getting notification logs: " << e.what() << std::endl;
    }
    
    return logs;
}

std::vector<domain::NotificationLog> NotificationRepo::getFailedNotifications() {
    std::vector<domain::NotificationLog> logs;
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        pqxx::result r = txn.exec_params(
            "SELECT id, notification_id, user_id, product_id, notification_type, message, status, retry_count, max_retries, error_message, sent_at, created_at, updated_at "
            "FROM notification_logs WHERE status = 'failed' AND retry_count < max_retries ORDER BY created_at ASC"
        );
        
        for (auto row : r) {
            domain::NotificationLog log;
            log.id = row["id"].as<int>();
            if (!row["notification_id"].is_null()) {
                log.notification_id = row["notification_id"].as<int>();
            }
            log.user_id = row["user_id"].as<int>();
            log.product_id = row["product_id"].as<int>();
            log.notification_type = row["notification_type"].as<std::string>();
            log.message = row["message"].as<std::string>();
            log.status = row["status"].as<std::string>();
            log.retry_count = row["retry_count"].as<int>();
            log.max_retries = row["max_retries"].as<int>();
            if (!row["error_message"].is_null()) {
                log.error_message = row["error_message"].as<std::string>();
            }
            if (!row["sent_at"].is_null()) {
                log.sent_at = row["sent_at"].as<std::string>();
            }
            log.created_at = row["created_at"].as<std::string>();
            log.updated_at = row["updated_at"].as<std::string>();
            
            logs.push_back(log);
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error getting failed notifications: " << e.what() << std::endl;
    }
    
    return logs;
}

bool NotificationRepo::incrementRetryCount(int log_id) {
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        txn.exec_params(
            "UPDATE notification_logs SET retry_count = retry_count + 1, updated_at = CURRENT_TIMESTAMP WHERE id = $1",
            log_id
        );
        
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error incrementing retry count: " << e.what() << std::endl;
        return false;
    }
}

bool NotificationRepo::createNotificationPreference(int user_id) {
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        // Check if preference already exists
        pqxx::result r = txn.exec_params(
            "SELECT id FROM notification_preferences WHERE user_id = $1",
            user_id
        );
        
        if (r.empty()) {
            txn.exec_params(
                "INSERT INTO notification_preferences (user_id, email_enabled, push_enabled, sms_enabled, in_app_enabled, created_at, updated_at) "
                "VALUES ($1, TRUE, FALSE, FALSE, TRUE, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)",
                user_id
            );
        }
        
        txn.commit();
        std::cout << "✅ Notification preferences created for user " << user_id << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error creating notification preference: " << e.what() << std::endl;
        return false;
    }
}

domain::NotificationPreference NotificationRepo::getNotificationPreference(int user_id) {
    domain::NotificationPreference pref;
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        pqxx::result r = txn.exec_params(
            "SELECT id, user_id, email_enabled, push_enabled, sms_enabled, in_app_enabled, created_at, updated_at "
            "FROM notification_preferences WHERE user_id = $1",
            user_id
        );
        
        if (!r.empty()) {
            auto row = r[0];
            pref.id = row["id"].as<int>();
            pref.user_id = row["user_id"].as<int>();
            pref.email_enabled = row["email_enabled"].as<bool>();
            pref.push_enabled = row["push_enabled"].as<bool>();
            pref.sms_enabled = row["sms_enabled"].as<bool>();
            pref.in_app_enabled = row["in_app_enabled"].as<bool>();
            pref.created_at = row["created_at"].as<std::string>();
            pref.updated_at = row["updated_at"].as<std::string>();
        } else {
            // Create default preference if not found
            createNotificationPreference(user_id);
            pref.user_id = user_id;
            pref.email_enabled = true;
            pref.in_app_enabled = true;
        }
    } catch (const std::exception& e) {
        std::cerr << "❌ Error getting notification preference: " << e.what() << std::endl;
    }
    
    return pref;
}

bool NotificationRepo::updateNotificationPreference(const domain::NotificationPreference& pref) {
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        txn.exec_params(
            "UPDATE notification_preferences SET email_enabled = $1, push_enabled = $2, sms_enabled = $3, in_app_enabled = $4, updated_at = CURRENT_TIMESTAMP "
            "WHERE user_id = $5",
            pref.email_enabled, pref.push_enabled, pref.sms_enabled, pref.in_app_enabled, pref.user_id
        );
        
        txn.commit();
        std::cout << "✅ Notification preferences updated for user " << pref.user_id << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error updating notification preference: " << e.what() << std::endl;
        return false;
    }
}

std::vector<domain::Notification> NotificationRepo::getPendingNotifications() {
    std::vector<domain::Notification> notifications;
    try {
        auto& conn = PostgresConnection::getConnection();
        pqxx::work txn(conn);
        
        pqxx::result r = txn.exec_params(
            "SELECT id, product_id, user_id, notification_type, is_sent, created_at, updated_at "
            "FROM product_notifications WHERE is_sent = FALSE ORDER BY created_at ASC"
        );
        
        for (auto row : r) {
            domain::Notification notif;
            notif.id = row["id"].as<int>();
            notif.product_id = row["product_id"].as<int>();
            notif.user_id = row["user_id"].as<int>();
            notif.type_str = row["notification_type"].as<std::string>();
            notif.is_sent = row["is_sent"].as<bool>();
            notif.created_at = row["created_at"].as<std::string>();
            notif.updated_at = row["updated_at"].as<std::string>();
            
            notifications.push_back(notif);
        }
        
        std::cout << "✅ Retrieved " << notifications.size() << " pending notifications" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "❌ Error getting pending notifications: " << e.what() << std::endl;
    }
    
    return notifications;
}
