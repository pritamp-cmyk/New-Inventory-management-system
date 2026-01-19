import React, { useState, useEffect } from 'react';
import '../styles/NotificationCenter.css';
import notificationService, { ProductNotification, NotificationLog } from '../services/notificationService';

interface NotificationCenterProps {
  userId: number;
  userName: string;
}

export const NotificationCenter: React.FC<NotificationCenterProps> = ({ userId, userName }) => {
  const [activeTab, setActiveTab] = useState<'subscriptions' | 'history' | 'preferences'>('subscriptions');
  const [subscriptions, setSubscriptions] = useState<ProductNotification[]>([]);
  const [history, setHistory] = useState<NotificationLog[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const [successMessage, setSuccessMessage] = useState<string | null>(null);
  const [emailEnabled, setEmailEnabled] = useState(true);
  const [pushEnabled, setPushEnabled] = useState(false);
  const [smsEnabled, setSmsEnabled] = useState(false);
  const [inAppEnabled, setInAppEnabled] = useState(true);

  useEffect(() => {
    loadNotifications();
    loadPreferences();
  }, [userId]);

  const loadNotifications = async () => {
    setLoading(true);
    setError(null);
    try {
      const subs = await notificationService.getUserNotifications(userId);
      setSubscriptions(subs);

      const hist = await notificationService.getNotificationLogs(userId);
      setHistory(hist);
    } catch (err) {
      setError('Failed to load notifications. Please try again.');
      console.error('❌ Error loading notifications:', err);
    } finally {
      setLoading(false);
    }
  };

  const loadPreferences = async () => {
    try {
      const prefs = await notificationService.getNotificationPreferences(userId);
      setEmailEnabled(prefs.email_enabled);
      setPushEnabled(prefs.push_enabled);
      setSmsEnabled(prefs.sms_enabled);
      setInAppEnabled(prefs.in_app_enabled);
    } catch (err) {
      console.error('❌ Error loading preferences:', err);
    }
  };

  const handleUnsubscribe = async (notificationId: number, productId: number) => {
    try {
      await notificationService.unsubscribeFromNotification(notificationId);
      setSuccessMessage(`Unsubscribed from product ${productId} notifications`);
      setSubscriptions(subscriptions.filter(s => s.id !== notificationId));
      setTimeout(() => setSuccessMessage(null), 3000);
    } catch (err) {
      setError('Failed to unsubscribe. Please try again.');
      console.error('❌ Error unsubscribing:', err);
    }
  };

  const handleResubscribe = async (productId: number) => {
    try {
      await notificationService.subscribeToProduct(userId, productId);
      setSuccessMessage(`Re-subscribed to product ${productId} notifications`);
      loadNotifications();
      setTimeout(() => setSuccessMessage(null), 3000);
    } catch (err) {
      setError('Failed to re-subscribe. Please try again.');
      console.error('❌ Error re-subscribing:', err);
    }
  };

  const handleSavePreferences = async () => {
    try {
      await notificationService.updateNotificationPreferences(userId, {
        email_enabled: emailEnabled,
        push_enabled: pushEnabled,
        sms_enabled: smsEnabled,
        in_app_enabled: inAppEnabled
      });
      setSuccessMessage('Notification preferences updated successfully');
      setTimeout(() => setSuccessMessage(null), 3000);
    } catch (err) {
      setError('Failed to save preferences. Please try again.');
      console.error('❌ Error saving preferences:', err);
    }
  };

  const handleRetryFailedNotification = async (logId: number) => {
    try {
      await notificationService.retryFailedNotification(logId);
      setSuccessMessage('Notification retry initiated');
      loadNotifications();
      setTimeout(() => setSuccessMessage(null), 3000);
    } catch (err) {
      setError('Failed to retry notification. Please try again.');
      console.error('❌ Error retrying notification:', err);
    }
  };

  const pendingSubscriptions = subscriptions.filter(s => !s.is_sent);
  const sentNotifications = subscriptions.filter(s => s.is_sent);
  const failedLogs = history.filter(h => h.status === 'failed');

  return (
    <div className="notification-center">
      <div className="notification-header">
        <h2>🔔 Notification Center</h2>
        <p className="user-info">Logged in as: {userName}</p>
      </div>

      {error && <div className="alert alert-error">❌ {error}</div>}
      {successMessage && <div className="alert alert-success">✅ {successMessage}</div>}

      <div className="notification-tabs">
        <button
          className={`tab ${activeTab === 'subscriptions' ? 'active' : ''}`}
          onClick={() => setActiveTab('subscriptions')}
        >
          📬 Subscriptions {pendingSubscriptions.length > 0 && `(${pendingSubscriptions.length})`}
        </button>
        <button
          className={`tab ${activeTab === 'history' ? 'active' : ''}`}
          onClick={() => setActiveTab('history')}
        >
          📋 History {history.length > 0 && `(${history.length})`}
        </button>
        <button
          className={`tab ${activeTab === 'preferences' ? 'active' : ''}`}
          onClick={() => setActiveTab('preferences')}
        >
          ⚙️ Preferences
        </button>
      </div>

      <div className="notification-content">
        {loading && <div className="loading">Loading...</div>}

        {/* Subscriptions Tab */}
        {activeTab === 'subscriptions' && !loading && (
          <div className="subscriptions-tab">
            <div className="section">
              <h3>🔔 Active Subscriptions (Awaiting Restock)</h3>
              {pendingSubscriptions.length === 0 ? (
                <p className="empty-state">No active subscriptions. Find an out-of-stock product to subscribe!</p>
              ) : (
                <div className="subscription-list">
                  {pendingSubscriptions.map(sub => (
                    <div key={sub.id} className="subscription-card">
                      <div className="subscription-info">
                        <h4>Product ID: {sub.product_id}</h4>
                        <p className="subscribed-date">
                          Subscribed: {new Date(sub.created_at).toLocaleDateString()}
                        </p>
                        <span className="badge badge-waiting">⏳ Waiting for Restock</span>
                      </div>
                      <button
                        className="btn btn-danger"
                        onClick={() => handleUnsubscribe(sub.id, sub.product_id)}
                      >
                        Unsubscribe
                      </button>
                    </div>
                  ))}
                </div>
              )}
            </div>

            <div className="section">
              <h3>✅ Already Notified (Available Again)</h3>
              {sentNotifications.length === 0 ? (
                <p className="empty-state">No notifications sent yet.</p>
              ) : (
                <div className="subscription-list">
                  {sentNotifications.map(sub => (
                    <div key={sub.id} className="subscription-card notified">
                      <div className="subscription-info">
                        <h4>Product ID: {sub.product_id}</h4>
                        <p className="notification-date">
                          Notified: {new Date(sub.sent_at || sub.created_at).toLocaleDateString()}
                        </p>
                        <span className="badge badge-success">✅ Notification Sent</span>
                      </div>
                      <button
                        className="btn btn-primary"
                        onClick={() => handleResubscribe(sub.product_id)}
                      >
                        Re-subscribe for Next Restock
                      </button>
                    </div>
                  ))}
                </div>
              )}
            </div>
          </div>
        )}

        {/* History Tab */}
        {activeTab === 'history' && !loading && (
          <div className="history-tab">
            {history.length === 0 ? (
              <p className="empty-state">No notification history yet.</p>
            ) : (
              <>
                {failedLogs.length > 0 && (
                  <div className="section">
                    <h3>⚠️ Failed Notifications ({failedLogs.length})</h3>
                    <div className="log-list">
                      {failedLogs.map(log => (
                        <div key={log.id} className="log-card failed">
                          <div className="log-header">
                            <h4>Product ID: {log.product_id}</h4>
                            <span className="badge badge-error">Failed</span>
                          </div>
                          <p className="log-message">{log.message}</p>
                          <p className="error-detail">Error: {log.error_message || 'Unknown error'}</p>
                          <div className="log-meta">
                            <span>Retries: {log.retry_count}/{log.max_retries}</span>
                            <span>Created: {new Date(log.created_at).toLocaleString()}</span>
                          </div>
                          {log.retry_count < log.max_retries && (
                            <button
                              className="btn btn-small btn-warning"
                              onClick={() => handleRetryFailedNotification(log.id)}
                            >
                              🔄 Retry
                            </button>
                          )}
                        </div>
                      ))}
                    </div>
                  </div>
                )}

                <div className="section">
                  <h3>📬 All Notifications</h3>
                  <div className="log-list">
                    {history.map(log => (
                      <div key={log.id} className={`log-card ${log.status}`}>
                        <div className="log-header">
                          <h4>Product ID: {log.product_id}</h4>
                          <span className={`badge badge-${log.status}`}>
                            {log.status === 'sent' && '✅ Sent'}
                            {log.status === 'pending' && '⏳ Pending'}
                            {log.status === 'failed' && '❌ Failed'}
                            {log.status === 'retried' && '🔄 Retried'}
                          </span>
                        </div>
                        <p className="log-message">{log.message}</p>
                        <div className="log-meta">
                          <span>Type: {log.type}</span>
                          <span>Retry: {log.retry_count}/{log.max_retries}</span>
                          <span>Created: {new Date(log.created_at).toLocaleString()}</span>
                          {log.sent_at && <span>Sent: {new Date(log.sent_at).toLocaleString()}</span>}
                        </div>
                      </div>
                    ))}
                  </div>
                </div>
              </>
            )}
          </div>
        )}

        {/* Preferences Tab */}
        {activeTab === 'preferences' && !loading && (
          <div className="preferences-tab">
            <h3>📧 Notification Channels</h3>
            <div className="preference-group">
              <label className="preference-item">
                <input
                  type="checkbox"
                  checked={emailEnabled}
                  onChange={(e) => setEmailEnabled(e.target.checked)}
                />
                <span className="preference-label">
                  📧 Email Notifications
                  <small>Receive notifications via email</small>
                </span>
              </label>

              <label className="preference-item">
                <input
                  type="checkbox"
                  checked={inAppEnabled}
                  onChange={(e) => setInAppEnabled(e.target.checked)}
                />
                <span className="preference-label">
                  🔔 In-App Notifications
                  <small>See notifications in the app</small>
                </span>
              </label>

              <label className="preference-item">
                <input
                  type="checkbox"
                  checked={pushEnabled}
                  onChange={(e) => setPushEnabled(e.target.checked)}
                />
                <span className="preference-label">
                  📱 Push Notifications
                  <small>Receive browser push notifications</small>
                </span>
              </label>

              <label className="preference-item">
                <input
                  type="checkbox"
                  checked={smsEnabled}
                  onChange={(e) => setSmsEnabled(e.target.checked)}
                />
                <span className="preference-label">
                  📞 SMS Notifications
                  <small>Receive notifications via SMS</small>
                </span>
              </label>
            </div>

            <button className="btn btn-primary btn-large" onClick={handleSavePreferences}>
              💾 Save Preferences
            </button>
          </div>
        )}
      </div>
    </div>
  );
};

export default NotificationCenter;
