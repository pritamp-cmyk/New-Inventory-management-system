import React, { useState, useEffect } from 'react';
import notificationService from '../services/notificationService';

interface NotificationBadgeProps {
  userId: number;
}

export const NotificationBadge: React.FC<NotificationBadgeProps> = ({ userId }) => {
  const [count, setCount] = useState(0);
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    loadUnreadCount();
    // Poll for updates every 30 seconds
    const interval = setInterval(loadUnreadCount, 30000);
    return () => clearInterval(interval);
  }, [userId]);

  const loadUnreadCount = async () => {
    try {
      setLoading(true);
      const notifications = await notificationService.getUserNotifications(userId);
      const sentButNotMarked = notifications.filter(n => n.is_sent).length;
      setCount(sentButNotMarked);
    } catch (err) {
      console.error('Error loading notification count:', err);
    } finally {
      setLoading(false);
    }
  };

  if (loading) {
    return <span className="notification-badge loading">...</span>;
  }

  return (
    <div className="notification-badge-container">
      <span className="notification-icon">🔔</span>
      {count > 0 && <span className="notification-badge">{count > 99 ? '99+' : count}</span>}
    </div>
  );
};

export default NotificationBadge;
