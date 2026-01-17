import React, { useState, useEffect } from 'react';
import { productAPI, userAPI, subscriptionAPI, inventoryAPI } from '../services/api';

interface DashboardStats {
  totalProducts: number;
  totalUsers: number;
  totalSubscriptions: number;
  lowStockProducts: number;
  activeSubscriptions: number;
}

const DashboardPage: React.FC = () => {
  const [stats, setStats] = useState<DashboardStats>({
    totalProducts: 0,
    totalUsers: 0,
    totalSubscriptions: 0,
    lowStockProducts: 0,
    activeSubscriptions: 0,
  });
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const [recentProducts, setRecentProducts] = useState<any[]>([]);
  const [lowStockAlerts, setLowStockAlerts] = useState<any[]>([]);

  useEffect(() => {
    loadDashboard();
  }, []);

  const loadDashboard = async () => {
    setLoading(true);
    setError('');

    try {
      const [products, users, subscriptions] = await Promise.all([
        productAPI.getAll(),
        userAPI.getAll(),
        subscriptionAPI.getAll(),
      ]);

      const productArray = Array.isArray(products) ? products : [];
      const userArray = Array.isArray(users) ? users : [];
      const subscriptionArray = Array.isArray(subscriptions) ? subscriptions : [];

      // Get inventory details for low stock check
      const lowStockProducts: any[] = [];
      for (const product of productArray) {
        try {
          const stock = await inventoryAPI.getStock(product.id);
          if (stock && stock.quantity < 50) {
            lowStockProducts.push({
              ...product,
              quantity: stock.quantity,
            });
          }
        } catch {
          // Continue if stock fetch fails
        }
      }

      const activeSubscriptions = subscriptionArray.filter((s: any) => s.active).length;

      setStats({
        totalProducts: productArray.length,
        totalUsers: userArray.length,
        totalSubscriptions: subscriptionArray.length,
        lowStockProducts: lowStockProducts.length,
        activeSubscriptions: activeSubscriptions,
      });

      setRecentProducts(productArray.slice(0, 5));
      setLowStockAlerts(lowStockProducts.slice(0, 5));
    } catch (err) {
      setError('Failed to load dashboard');
    } finally {
      setLoading(false);
    }
  };

  const StatCard = ({
    icon,
    label,
    value,
    color,
  }: {
    icon: string;
    label: string;
    value: number;
    color: string;
  }) => (
    <div className="stat-card" style={{ borderLeftColor: color }}>
      <div className="stat-icon">{icon}</div>
      <div className="stat-content">
        <div className="stat-label">{label}</div>
        <div className="stat-value">{value}</div>
      </div>
    </div>
  );

  return (
    <div className="card">
      <h1 className="card-title">📊 Dashboard</h1>

      {error && <div className="alert alert-danger">{error}</div>}

      <div style={{ marginBottom: '2rem' }}>
        <button className="btn btn-info" onClick={loadDashboard} disabled={loading}>
          {loading ? 'Refreshing...' : '🔄 Refresh'}
        </button>
      </div>

      {/* Stats Grid */}
      {loading && !stats.totalProducts ? (
        <div className="loading">Loading dashboard...</div>
      ) : (
        <>
          <div className="stats-grid">
            <StatCard
              icon="📦"
              label="Total Products"
              value={stats.totalProducts}
              color="#3498db"
            />
            <StatCard
              icon="👥"
              label="Total Users"
              value={stats.totalUsers}
              color="#2ecc71"
            />
            <StatCard
              icon="📋"
              label="Total Subscriptions"
              value={stats.totalSubscriptions}
              color="#9b59b6"
            />
            <StatCard
              icon="✓"
              label="Active Subscriptions"
              value={stats.activeSubscriptions}
              color="#27ae60"
            />
            <StatCard
              icon="⚠️"
              label="Low Stock Products"
              value={stats.lowStockProducts}
              color="#e74c3c"
            />
          </div>

          {/* Recent Products Section */}
          <div style={{ marginTop: '3rem' }}>
            <h2>📦 Recent Products</h2>
            {recentProducts.length === 0 ? (
              <div className="empty-state">
                <div className="empty-state-icon">📭</div>
                <p>No products found</p>
              </div>
            ) : (
              <div className="products-preview">
                {recentProducts.map((product) => (
                  <div key={product.id} className="product-preview-card">
                    <div className="product-preview-id">#{product.id}</div>
                    <h3>{product.name}</h3>
                    <p className="product-preview-desc">{product.description}</p>
                    <div className="product-preview-price">
                      ${product.price?.toFixed(2) || '0.00'}
                    </div>
                  </div>
                ))}
              </div>
            )}
          </div>

          {/* Low Stock Alerts Section */}
          <div style={{ marginTop: '3rem' }}>
            <h2>⚠️ Low Stock Alerts</h2>
            {lowStockAlerts.length === 0 ? (
              <div className="alert alert-success">✓ All products are well stocked!</div>
            ) : (
              <div style={{ overflowX: 'auto' }}>
                <table>
                  <thead>
                    <tr>
                      <th>Product ID</th>
                      <th>Product Name</th>
                      <th>Current Stock</th>
                      <th>Status</th>
                    </tr>
                  </thead>
                  <tbody>
                    {lowStockAlerts.map((product) => (
                      <tr key={product.id}>
                        <td>#{product.id}</td>
                        <td>
                          <strong>{product.name}</strong>
                        </td>
                        <td>
                          <span
                            style={{
                              fontWeight: 'bold',
                              color: product.quantity < 20 ? '#e74c3c' : '#f39c12',
                            }}
                          >
                            {product.quantity} units
                          </span>
                        </td>
                        <td>
                          {product.quantity < 20 ? (
                            <span className="badge badge-danger">🔴 Critical</span>
                          ) : (
                            <span className="badge badge-warning">🟡 Low</span>
                          )}
                        </td>
                      </tr>
                    ))}
                  </tbody>
                </table>
              </div>
            )}
          </div>

          {/* Quick Actions */}
          <div className="quick-actions" style={{ marginTop: '3rem' }}>
            <h2>⚡ Quick Actions</h2>
            <div className="quick-actions-grid">
              <button className="btn btn-success btn-lg">
                ➕ Add Product
              </button>
              <button className="btn btn-info btn-lg">
                👥 Add User
              </button>
              <button className="btn btn-primary btn-lg">
                📋 Create Subscription
              </button>
              <button className="btn btn-warning btn-lg">
                📥 Import Data
              </button>
            </div>
          </div>
        </>
      )}
    </div>
  );
};

export default DashboardPage;
