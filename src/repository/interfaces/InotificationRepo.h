#pragma once
#include "../../domain/notification.h"
#include <vector>
#include <string>

class InotificationRepo {
public:
    virtual ~InotificationRepo() = default;
    
    // Notification subscription operations
    virtual bool subscribeToNotification(int user_id, int product_id, const std::string& type) = 0;
    virtual bool unsubscribeFromNotification(int notification_id) = 0;
    virtual std::vector<domain::Notification> getUserNotifications(int user_id) = 0;
    virtual std::vector<domain::Notification> getProductSubscribers(int product_id) = 0;
    virtual domain::Notification getNotificationById(int notification_id) = 0;
    
    // Check if user is subscribed to product
    virtual bool isUserSubscribed(int user_id, int product_id) = 0;
    virtual int getSubscriptionId(int user_id, int product_id) = 0;
    
    // Mark notification as sent
    virtual bool markNotificationAsSent(int notification_id) = 0;
    
    // Notification logging operations (fault tolerance)
    virtual int createNotificationLog(const domain::NotificationLog& log) = 0;
    virtual bool updateNotificationLogStatus(int log_id, const std::string& status, const std::string& message = "") = 0;
    virtual std::vector<domain::NotificationLog> getNotificationLogs(int user_id, const std::string& status = "") = 0;
    virtual std::vector<domain::NotificationLog> getFailedNotifications() = 0;
    virtual bool incrementRetryCount(int log_id) = 0;
    
    // Notification preferences
    virtual bool createNotificationPreference(int user_id) = 0;
    virtual domain::NotificationPreference getNotificationPreference(int user_id) = 0;
    virtual bool updateNotificationPreference(const domain::NotificationPreference& pref) = 0;
    
    // Get all pending notifications (unsent)
    virtual std::vector<domain::Notification> getPendingNotifications() = 0;
};
