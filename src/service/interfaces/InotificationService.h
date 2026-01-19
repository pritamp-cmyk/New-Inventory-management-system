#pragma once
#include "../../domain/notification.h"
#include <vector>
#include <string>

class InotificationService {
public:
    virtual ~InotificationService() = default;
    
    // Subscribe/Unsubscribe operations
    virtual bool subscribeUser(int user_id, int product_id) = 0;
    virtual bool unsubscribeUser(int notification_id) = 0;
    
    // Get notifications
    virtual std::vector<domain::Notification> getUserNotifications(int user_id) = 0;
    virtual std::vector<domain::Notification> getProductSubscribers(int product_id) = 0;
    
    // Notification sending
    virtual bool sendRestockNotifications(int product_id, int stock) = 0;
    virtual bool sendNotification(int notification_id, const std::string& message) = 0;
    
    // Get notification logs
    virtual std::vector<domain::NotificationLog> getNotificationLogs(int user_id) = 0;
    virtual std::vector<domain::NotificationLog> getFailedNotifications() = 0;
    
    // Retry failed notifications
    virtual bool retryFailedNotification(int log_id) = 0;
    
    // Preferences
    virtual bool updateUserPreferences(int user_id, bool email, bool push, bool sms, bool in_app) = 0;
    virtual domain::NotificationPreference getUserPreferences(int user_id) = 0;
};
