-- Database schema migration for notification system
-- Run this script against inventory_db to add notification support

-- Table 1: Product Notifications (tracks subscriptions)
CREATE TABLE IF NOT EXISTS product_notifications (
    id SERIAL PRIMARY KEY,
    product_id INT NOT NULL,
    user_id INT NOT NULL,
    notification_type VARCHAR(50) DEFAULT 'restocked' CHECK (notification_type IN ('out_of_stock', 'restocked')),
    is_sent BOOLEAN DEFAULT FALSE,
    sent_at TIMESTAMP,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (product_id) REFERENCES products(id) ON DELETE CASCADE,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    UNIQUE(product_id, user_id, notification_type)
);

-- Table 2: Notification Logs (audit and fault recovery)
CREATE TABLE IF NOT EXISTS notification_logs (
    id SERIAL PRIMARY KEY,
    notification_id INT,
    user_id INT NOT NULL,
    product_id INT NOT NULL,
    notification_type VARCHAR(50) DEFAULT 'restocked',
    message TEXT,
    status VARCHAR(50) DEFAULT 'pending' CHECK (status IN ('pending', 'sent', 'failed', 'retried')),
    retry_count INT DEFAULT 0,
    max_retries INT DEFAULT 3,
    error_message TEXT,
    sent_at TIMESTAMP,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (notification_id) REFERENCES product_notifications(id) ON DELETE SET NULL,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    FOREIGN KEY (product_id) REFERENCES products(id) ON DELETE CASCADE
);

-- Table 3: User Notification Preferences
CREATE TABLE IF NOT EXISTS notification_preferences (
    id SERIAL PRIMARY KEY,
    user_id INT NOT NULL UNIQUE,
    email_enabled BOOLEAN DEFAULT TRUE,
    push_enabled BOOLEAN DEFAULT FALSE,
    sms_enabled BOOLEAN DEFAULT FALSE,
    in_app_enabled BOOLEAN DEFAULT TRUE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);

-- Create indexes for better query performance
CREATE INDEX IF NOT EXISTS idx_product_notifications_product_id ON product_notifications(product_id);
CREATE INDEX IF NOT EXISTS idx_product_notifications_user_id ON product_notifications(user_id);
CREATE INDEX IF NOT EXISTS idx_product_notifications_sent ON product_notifications(is_sent);
CREATE INDEX IF NOT EXISTS idx_product_notifications_created_at ON product_notifications(created_at);

CREATE INDEX IF NOT EXISTS idx_notification_logs_status ON notification_logs(status);
CREATE INDEX IF NOT EXISTS idx_notification_logs_user_id ON notification_logs(user_id);
CREATE INDEX IF NOT EXISTS idx_notification_logs_product_id ON notification_logs(product_id);
CREATE INDEX IF NOT EXISTS idx_notification_logs_created_at ON notification_logs(created_at);

CREATE INDEX IF NOT EXISTS idx_notification_preferences_user_id ON notification_preferences(user_id);

-- View for finding out-of-stock products with subscribers
CREATE OR REPLACE VIEW v_out_of_stock_subscriptions AS
SELECT 
    pn.id as notification_id,
    pn.product_id,
    pn.user_id,
    p.name as product_name,
    u.name as user_name,
    u.email,
    i.stock,
    pn.created_at as subscribed_at
FROM product_notifications pn
JOIN products p ON p.id = pn.product_id
JOIN users u ON u.id = pn.user_id
LEFT JOIN inventory i ON i.product_id = pn.product_id
WHERE pn.is_sent = FALSE AND i.stock = 0;

-- View for finding products that were just restocked
CREATE OR REPLACE VIEW v_restocked_products AS
SELECT 
    p.id,
    p.name,
    i.stock,
    i.updated_at,
    COUNT(pn.id) as subscriber_count
FROM products p
JOIN inventory i ON i.product_id = p.id
LEFT JOIN product_notifications pn ON pn.product_id = p.id AND pn.is_sent = FALSE
WHERE i.stock > 0
GROUP BY p.id, p.name, i.stock, i.updated_at;

COMMIT;
