const API_BASE_URL = process.env.REACT_APP_API_BASE_URL || 'http://localhost:8080';
const API_URL = `${API_BASE_URL}/api`;

export interface ProductNotification {
  id: number;
  product_id: number;
  user_id: number;
  type: string;
  is_sent: boolean;
  created_at: string;
  sent_at?: string;
}

export interface NotificationLog {
  id: number;
  notification_id: number;
  user_id: number;
  product_id: number;
  type: string;
  message: string;
  status: 'pending' | 'sent' | 'failed' | 'retried';
  retry_count: number;
  max_retries: number;
  error_message?: string;
  sent_at?: string;
  created_at: string;
}

export interface NotificationPreference {
  id: number;
  user_id: number;
  email_enabled: boolean;
  push_enabled: boolean;
  sms_enabled: boolean;
  in_app_enabled: boolean;
  created_at: string;
  updated_at: string;
}

class NotificationService {
  /**
   * Subscribe a user to product restock notifications
   */
  async subscribeToProduct(userId: number, productId: number): Promise<ProductNotification> {
    try {
      const response = await fetch(`${API_URL}/notifications/subscribe`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ user_id: userId, product_id: productId })
      });
      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
      }
      const data = await response.json();
      console.log('✅ Subscribed to product notifications:', data);
      return data;
    } catch (error) {
      console.error('❌ Error subscribing to product:', error);
      throw error;
    }
  }

  /**
   * Unsubscribe from a specific notification
   */
  async unsubscribeFromNotification(notificationId: number): Promise<{ status: string; message: string }> {
    try {
      const response = await fetch(`${API_URL}/notifications/${notificationId}`, {
        method: 'DELETE'
      });
      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
      }
      const data = await response.json();
      console.log('✅ Unsubscribed from notifications:', data);
      return data;
    } catch (error) {
      console.error('❌ Error unsubscribing:', error);
      throw error;
    }
  }

  /**
   * Get all notifications for a user
   */
  async getUserNotifications(userId: number): Promise<ProductNotification[]> {
    try {
      const response = await fetch(`${API_URL}/notifications/user/${userId}`);
      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
      }
      const data = await response.json();
      console.log('✅ Retrieved user notifications:', data);
      return data;
    } catch (error) {
      console.error('❌ Error getting user notifications:', error);
      throw error;
    }
  }

  /**
   * Get all subscribers for a product
   */
  async getProductSubscribers(productId: number): Promise<ProductNotification[]> {
    try {
      const response = await fetch(`${API_URL}/notifications/product/${productId}`);
      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
      }
      const data = await response.json();
      console.log('✅ Retrieved product subscribers:', data);
      return data;
    } catch (error) {
      console.error('❌ Error getting product subscribers:', error);
      throw error;
    }
  }

  /**
   * Get notification logs for a user
   */
  async getNotificationLogs(userId: number): Promise<NotificationLog[]> {
    try {
      const response = await fetch(`${API_URL}/notifications/logs/user/${userId}`);
      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
      }
      const data = await response.json();
      console.log('✅ Retrieved notification logs:', data);
      return data;
    } catch (error) {
      console.error('❌ Error getting notification logs:', error);
      throw error;
    }
  }

  /**
   * Get all failed notifications in the system
   */
  async getFailedNotifications(): Promise<NotificationLog[]> {
    try {
      const response = await fetch(`${API_URL}/notifications/logs/status/failed`);
      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
      }
      const data = await response.json();
      console.log('✅ Retrieved failed notifications:', data);
      return data;
    } catch (error) {
      console.error('❌ Error getting failed notifications:', error);
      throw error;
    }
  }

  /**
   * Retry sending a failed notification
   */
  async retryFailedNotification(logId: number): Promise<{ status: string; message: string }> {
    try {
      const response = await fetch(`${API_URL}/notifications/logs/${logId}/retry`, {
        method: 'POST'
      });
      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
      }
      const data = await response.json();
      console.log('✅ Notification retry initiated:', data);
      return data;
    } catch (error) {
      console.error('❌ Error retrying notification:', error);
      throw error;
    }
  }

  /**
   * Update user notification preferences
   */
  async updateNotificationPreferences(
    userId: number,
    preferences: Partial<Omit<NotificationPreference, 'id' | 'user_id' | 'created_at' | 'updated_at'>>
  ): Promise<{ status: string; message: string }> {
    try {
      const response = await fetch(`${API_URL}/notifications/preferences/${userId}`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(preferences)
      });
      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
      }
      const data = await response.json();
      console.log('✅ Notification preferences updated:', data);
      return data;
    } catch (error) {
      console.error('❌ Error updating preferences:', error);
      throw error;
    }
  }

  /**
   * Get user notification preferences
   */
  async getNotificationPreferences(userId: number): Promise<NotificationPreference> {
    try {
      const response = await fetch(`${API_URL}/notifications/preferences/${userId}`);
      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
      }
      const data = await response.json();
      console.log('✅ Retrieved notification preferences:', data);
      return data;
    } catch (error) {
      console.error('❌ Error getting preferences:', error);
      throw error;
    }
  }

  /**
   * Check if user is subscribed to a product
   */
  async isUserSubscribed(userId: number, productId: number): Promise<boolean> {
    try {
      const notifications = await this.getUserNotifications(userId);
      return notifications.some(n => n.product_id === productId && !n.is_sent);
    } catch {
      return false;
    }
  }

  /**
   * Get subscription for a specific product
   */
  async getProductSubscription(userId: number, productId: number): Promise<ProductNotification | null> {
    try {
      const notifications = await this.getUserNotifications(userId);
      return notifications.find(n => n.product_id === productId) || null;
    } catch {
      return null;
    }
  }

  /**
   * Get unread notification count for user
   */
  async getUnreadNotificationCount(userId: number): Promise<number> {
    try {
      const logs = await this.getNotificationLogs(userId);
      return logs.filter(l => l.status === 'sent' && !l.sent_at).length;
    } catch {
      return 0;
    }
  }
}

export default new NotificationService();
