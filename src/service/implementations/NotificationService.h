#pragma once
#include "../interfaces/InotificationService.h"
#include "../../repository/interfaces/InotificationRepo.h"
#include <memory>

class NotificationService : public InotificationService {
private:
    std::unique_ptr<InotificationRepo> notification_repo;
    
    // Helper method to simulate sending notifications
    bool simulateSendNotification(int user_id, int product_id, 
                                 const std::string& message,
                                 const domain::NotificationPreference& prefs);
    
public:
    NotificationService();
    
    // Subscribe/Unsubscribe operations
    bool subscribeUser(int user_id, int product_id) override;
    bool unsubscribeUser(int notification_id) override;
    
    // Get notifications
    std::vector<domain::Notification> getUserNotifications(int user_id) override;
    std::vector<domain::Notification> getProductSubscribers(int product_id) override;
    
    // Notification sending
    bool sendRestockNotifications(int product_id, int stock) override;
    bool sendNotification(int notification_id, const std::string& message) override;
    
    // Get notification logs
    std::vector<domain::NotificationLog> getNotificationLogs(int user_id) override;
    std::vector<domain::NotificationLog> getFailedNotifications() override;
    
    // Retry failed notifications
    bool retryFailedNotification(int log_id) override;
    
    // Preferences
    bool updateUserPreferences(int user_id, bool email, bool push, bool sms, bool in_app) override;
    domain::NotificationPreference getUserPreferences(int user_id) override;
};
