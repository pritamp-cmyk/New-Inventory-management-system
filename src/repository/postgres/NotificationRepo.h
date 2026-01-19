#pragma once
#include "../interfaces/InotificationRepo.h"

class NotificationRepo : public InotificationRepo {
public:
    NotificationRepo();
    
    // Notification subscription operations
    bool subscribeToNotification(int user_id, int product_id, const std::string& type) override;
    bool unsubscribeFromNotification(int notification_id) override;
    std::vector<domain::Notification> getUserNotifications(int user_id) override;
    std::vector<domain::Notification> getProductSubscribers(int product_id) override;
    domain::Notification getNotificationById(int notification_id) override;
    
    // Check if user is subscribed to product
    bool isUserSubscribed(int user_id, int product_id) override;
    int getSubscriptionId(int user_id, int product_id) override;
    
    // Mark notification as sent
    bool markNotificationAsSent(int notification_id) override;
    
    // Notification logging operations
    int createNotificationLog(const domain::NotificationLog& log) override;
    bool updateNotificationLogStatus(int log_id, const std::string& status, const std::string& message = "") override;
    std::vector<domain::NotificationLog> getNotificationLogs(int user_id, const std::string& status = "") override;
    std::vector<domain::NotificationLog> getFailedNotifications() override;
    bool incrementRetryCount(int log_id) override;
    
    // Notification preferences
    bool createNotificationPreference(int user_id) override;
    domain::NotificationPreference getNotificationPreference(int user_id) override;
    bool updateNotificationPreference(const domain::NotificationPreference& pref) override;
    
    // Get all pending notifications
    std::vector<domain::Notification> getPendingNotifications() override;
};
