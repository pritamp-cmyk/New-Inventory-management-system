#pragma once
#include <string>
#include <ctime>

namespace domain {

enum class NotificationType {
    OUT_OF_STOCK,
    RESTOCKED
};

enum class NotificationStatus {
    PENDING,
    SENT,
    FAILED,
    RETRIED
};

struct Notification {
    int id;
    int product_id;
    int user_id;
    NotificationType type;
    std::string type_str;
    bool is_sent;
    std::string created_at;
    std::string updated_at;
    std::string sent_at;
    
    Notification() : id(0), product_id(0), user_id(0), 
                     type(NotificationType::RESTOCKED), 
                     is_sent(false) {}
    
    Notification(int id, int product_id, int user_id, 
                 const std::string& type_str, bool is_sent,
                 const std::string& created_at, const std::string& updated_at)
        : id(id), product_id(product_id), user_id(user_id), 
          type_str(type_str), is_sent(is_sent),
          created_at(created_at), updated_at(updated_at) {
        
        if (type_str == "out_of_stock") {
            type = NotificationType::OUT_OF_STOCK;
        } else {
            type = NotificationType::RESTOCKED;
        }
    }
};

struct NotificationLog {
    int id;
    int notification_id;
    int user_id;
    int product_id;
    std::string notification_type;
    std::string message;
    std::string status;
    int retry_count;
    int max_retries;
    std::string error_message;
    std::string sent_at;
    std::string created_at;
    std::string updated_at;
    
    NotificationLog() : id(0), notification_id(0), user_id(0), product_id(0),
                        retry_count(0), max_retries(3) {}
};

struct NotificationPreference {
    int id;
    int user_id;
    bool email_enabled;
    bool push_enabled;
    bool sms_enabled;
    bool in_app_enabled;
    std::string created_at;
    std::string updated_at;
    
    NotificationPreference() : id(0), user_id(0), 
                              email_enabled(true), push_enabled(false),
                              sms_enabled(false), in_app_enabled(true) {}
};

} // namespace domain
