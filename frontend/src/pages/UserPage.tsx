import React, { useState, useEffect } from 'react';
import { userAPI } from '../services/api';

interface User {
  id: number;
  name: string;
  email: string;
  role: 'admin' | 'user';
}

const UserPage: React.FC = () => {
  const [users, setUsers] = useState<User[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const [showForm, setShowForm] = useState(false);
  const [editingId, setEditingId] = useState<number | null>(null);

  const [formData, setFormData] = useState({
    name: '',
    email: '',
    role: 'user' as 'admin' | 'user',
  });

  useEffect(() => {
    loadUsers();
  }, []);

  const loadUsers = async () => {
    setLoading(true);
    setError('');
    try {
      const data = await userAPI.getAll();
      setUsers(Array.isArray(data) ? data : []);
    } catch (err) {
      setError('Failed to load users');
    } finally {
      setLoading(false);
    }
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();

    if (!formData.name.trim() || !formData.email.trim()) {
      setError('Please fill in all fields');
      return;
    }

    setLoading(true);
    setError('');

    try {
      if (editingId) {
        const result = await userAPI.update(editingId, formData.name, formData.email, formData.role);
        console.log('Update result:', result);
      } else {
        const result = await userAPI.create(formData.name, formData.email, formData.role);
        console.log('Create result:', result);
      }

      setFormData({ name: '', email: '', role: 'user' });
      setEditingId(null);
      setShowForm(false);
      await loadUsers();
    } catch (err: any) {
      console.error('Save error:', err);
      setError(err.message || 'Failed to save user');
    } finally {
      setLoading(false);
    }
  };

  const handleEdit = (user: User) => {
    setFormData({
      name: user.name,
      email: user.email,
      role: user.role,
    });
    setEditingId(user.id);
    setShowForm(true);
  };

  const handleDelete = async (id: number) => {
    if (!window.confirm('Are you sure you want to delete this user?')) {
      return;
    }

    setLoading(true);
    setError('');
    try {
      const result = await userAPI.delete(id);
      console.log('Delete result:', result);
      await loadUsers();
    } catch (err: any) {
      console.error('Delete error:', err);
      setError(err.message || 'Failed to delete user');
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="card">
      <h1 className="card-title">👥 Users Management</h1>

      {error && <div className="alert alert-danger">{error}</div>}

      <div style={{ marginBottom: '2rem' }}>
        <button
          className="btn btn-success"
          onClick={() => {
            setShowForm(!showForm);
            setEditingId(null);
            setFormData({ name: '', email: '', role: 'user' });
          }}
        >
          {showForm ? 'Cancel' : '➕ Add User'}
        </button>
      </div>

      {/* Add/Edit Form */}
      {showForm && (
        <div className="card" style={{ background: '#f9f9f9', marginBottom: '2rem' }}>
          <h3>{editingId ? 'Edit User' : 'New User'}</h3>
          <form onSubmit={handleSubmit}>
            <div className="form-group">
              <label>Full Name</label>
              <input
                type="text"
                value={formData.name}
                onChange={(e) => setFormData({ ...formData, name: e.target.value })}
                placeholder="e.g., John Doe"
                required
              />
            </div>

            <div className="form-group">
              <label>Email</label>
              <input
                type="email"
                value={formData.email}
                onChange={(e) => setFormData({ ...formData, email: e.target.value })}
                placeholder="e.g., john@test.com"
                required
              />
            </div>

            <div className="form-group">
              <label>Role</label>
              <select
                value={formData.role}
                onChange={(e) => setFormData({ ...formData, role: e.target.value as 'admin' | 'user' })}
              >
                <option value="user">User</option>
                <option value="admin">Admin</option>
              </select>
            </div>

            <button type="submit" className="btn btn-success" disabled={loading}>
              {loading ? 'Saving...' : editingId ? 'Update' : 'Create'}
            </button>
          </form>
        </div>
      )}

      {/* Users Table */}
      {loading && !users.length ? (
        <div className="loading">Loading users...</div>
      ) : users.length === 0 ? (
        <div className="empty-state">
          <div className="empty-state-icon">👤</div>
          <p>No users found</p>
        </div>
      ) : (
        <div style={{ overflowX: 'auto' }}>
          <table>
            <thead>
              <tr>
                <th>ID</th>
                <th>Name</th>
                <th>Email</th>
                <th>Role</th>
                <th>Actions</th>
              </tr>
            </thead>
            <tbody>
              {users.map((user) => (
                <tr key={user.id}>
                  <td>#{user.id}</td>
                  <td><strong>{user.name}</strong></td>
                  <td>{user.email}</td>
                  <td>
                    <span className={user.role === 'admin' ? 'badge badge-warning' : 'badge badge-info'}>
                      {user.role.toUpperCase()}
                    </span>
                  </td>
                  <td>
                    <div className="btn-group">
                      <button
                        className="btn btn-primary btn-small"
                        onClick={() => handleEdit(user)}
                      >
                        ✏️ Edit
                      </button>
                      <button
                        className="btn btn-danger btn-small"
                        onClick={() => handleDelete(user.id)}
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

export default UserPage;
