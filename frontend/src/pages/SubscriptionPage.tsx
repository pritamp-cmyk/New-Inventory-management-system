import React, { useState, useEffect } from 'react';
import { subscriptionAPI, userAPI, productAPI } from '../services/api';

interface Subscription {
  id: number;
  user_id: number;
  product_id: number;
  active: boolean;
  created_at?: string;
}

interface User {
  id: number;
  name: string;
}

interface Product {
  id: number;
  name: string;
}

const SubscriptionPage: React.FC = () => {
  const [subscriptions, setSubscriptions] = useState<Subscription[]>([]);
  const [users, setUsers] = useState<User[]>([]);
  const [products, setProducts] = useState<Product[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const [showForm, setShowForm] = useState(false);

  const [formData, setFormData] = useState({
    user_id: '',
    product_id: '',
  });

  useEffect(() => {
    loadData();
  }, []);

  const loadData = async () => {
    setLoading(true);
    setError('');
    try {
      const [subData, userData, productData] = await Promise.all([
        subscriptionAPI.getAll(),
        userAPI.getAll(),
        productAPI.getAll(),
      ]);

      setSubscriptions(Array.isArray(subData) ? subData : []);
      setUsers(Array.isArray(userData) ? userData : []);
      setProducts(Array.isArray(productData) ? productData : []);
    } catch (err) {
      setError('Failed to load data');
    } finally {
      setLoading(false);
    }
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();

    if (!formData.user_id || !formData.product_id) {
      setError('Please select both user and product');
      return;
    }

    setLoading(true);
    setError('');

    try {
      const result = await subscriptionAPI.create(
        parseInt(formData.user_id),
        parseInt(formData.product_id)
      );
      console.log('Subscription created:', result);

      setFormData({ user_id: '', product_id: '' });
      setShowForm(false);
      await loadData();
    } catch (err: any) {
      console.error('Save error:', err);
      setError(err.message || 'Failed to create subscription');
    } finally {
      setLoading(false);
    }
  };

  const handleToggleActive = async (id: number, active: boolean) => {
    try {
      const result = await subscriptionAPI.updateStatus(id, !active);
      console.log('Status updated:', result);
      await loadData();
    } catch (err: any) {
      console.error('Update error:', err);
      setError(err.message || 'Failed to update subscription');
    }
  };

  const handleDelete = async (id: number) => {
    if (!window.confirm('Are you sure you want to unsubscribe?')) {
      return;
    }

    setLoading(true);
    setError('');

    try {
      const result = await subscriptionAPI.delete(id);
      console.log('Subscription deleted:', result);
      await loadData();
    } catch (err: any) {
      console.error('Delete error:', err);
      setError(err.message || 'Failed to delete subscription');
    } finally {
      setLoading(false);
    }
  };

  const getUserName = (userId: number) => {
    return users.find(u => u.id === userId)?.name || `User #${userId}`;
  };

  const getProductName = (productId: number) => {
    return products.find(p => p.id === productId)?.name || `Product #${productId}`;
  };

  return (
    <div className="card">
      <h1 className="card-title">📋 Subscriptions Management</h1>

      {error && <div className="alert alert-danger">{error}</div>}

      <div style={{ marginBottom: '2rem' }}>
        <button
          className="btn btn-success"
          onClick={() => {
            setShowForm(!showForm);
            setFormData({ user_id: '', product_id: '' });
          }}
        >
          {showForm ? 'Cancel' : '➕ New Subscription'}
        </button>
      </div>

      {/* Add Form */}
      {showForm && (
        <div className="card" style={{ background: '#f9f9f9', marginBottom: '2rem' }}>
          <h3>Create New Subscription</h3>
          <form onSubmit={handleSubmit}>
            <div className="form-group">
              <label>User</label>
              <select
                value={formData.user_id}
                onChange={(e) => setFormData({ ...formData, user_id: e.target.value })}
                required
              >
                <option value="">Select a user...</option>
                {users.map((user) => (
                  <option key={user.id} value={user.id}>
                    {user.name}
                  </option>
                ))}
              </select>
            </div>

            <div className="form-group">
              <label>Product</label>
              <select
                value={formData.product_id}
                onChange={(e) => setFormData({ ...formData, product_id: e.target.value })}
                required
              >
                <option value="">Select a product...</option>
                {products.map((product) => (
                  <option key={product.id} value={product.id}>
                    {product.name}
                  </option>
                ))}
              </select>
            </div>

            <button type="submit" className="btn btn-success" disabled={loading}>
              {loading ? 'Creating...' : 'Create Subscription'}
            </button>
          </form>
        </div>
      )}

      {/* Subscriptions Table */}
      {loading && !subscriptions.length ? (
        <div className="loading">Loading subscriptions...</div>
      ) : subscriptions.length === 0 ? (
        <div className="empty-state">
          <div className="empty-state-icon">📭</div>
          <p>No subscriptions found</p>
        </div>
      ) : (
        <div style={{ overflowX: 'auto' }}>
          <table>
            <thead>
              <tr>
                <th>ID</th>
                <th>User</th>
                <th>Product</th>
                <th>Status</th>
                <th>Date</th>
                <th>Actions</th>
              </tr>
            </thead>
            <tbody>
              {subscriptions.map((sub) => (
                <tr key={sub.id}>
                  <td>#{sub.id}</td>
                  <td><strong>{getUserName(sub.user_id)}</strong></td>
                  <td>{getProductName(sub.product_id)}</td>
                  <td>
                    <span className={sub.active ? 'badge badge-success' : 'badge badge-danger'}>
                      {sub.active ? '✓ Active' : '✗ Inactive'}
                    </span>
                  </td>
                  <td>{sub.created_at ? new Date(sub.created_at).toLocaleDateString() : 'N/A'}</td>
                  <td>
                    <div className="btn-group">
                      <button
                        className="btn btn-primary btn-small"
                        onClick={() => handleToggleActive(sub.id, sub.active)}
                      >
                        {sub.active ? '⏸️ Pause' : '▶️ Resume'}
                      </button>
                      <button
                        className="btn btn-danger btn-small"
                        onClick={() => handleDelete(sub.id)}
                      >
                        🗑️ Delete
                      </button>
                    </div>
                  </td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      )}
    </div>
  );
};

export default SubscriptionPage;
