import React, { useState, useEffect } from 'react';
import { productAPI, inventoryAPI } from '../services/api';

interface Product {
  id: number;
  name: string;
  description: string;
  price?: number;
}

interface Inventory {
  product_id: number;
  stock: number;
}

const ProductPage: React.FC = () => {
  const [products, setProducts] = useState<Product[]>([]);
  const [inventory, setInventory] = useState<{ [key: number]: number }>({});
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const [searchTerm, setSearchTerm] = useState('');
  const [showForm, setShowForm] = useState(false);
  const [editingId, setEditingId] = useState<number | null>(null);

  // Form state
  const [formData, setFormData] = useState({
    name: '',
    description: '',
    initial_stock: '50',
  });

  // Load products on mount
  useEffect(() => {
    loadProducts();
  }, []);

  const loadProducts = async () => {
    setLoading(true);
    setError('');
    try {
      const data = await productAPI.getAll();
      setProducts(Array.isArray(data) ? data : []);
      
      // Load inventory for each product
      if (Array.isArray(data)) {
        for (const product of data) {
          try {
            const inv = await inventoryAPI.getStock(product.id);
            setInventory(prev => ({ ...prev, [product.id]: inv.stock || 0 }));
          } catch (err) {
            // Continue if stock not found
          }
        }
      }
    } catch (err) {
      setError('Failed to load products');
    } finally {
      setLoading(false);
    }
  };

  const handleSearch = async () => {
    if (!searchTerm.trim()) {
      loadProducts();
      return;
    }

    setLoading(true);
    setError('');
    try {
      const data = await productAPI.search(searchTerm);
      setProducts(Array.isArray(data) ? data : []);
    } catch (err) {
      setError('Search failed');
    } finally {
      setLoading(false);
    }
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    
    if (!formData.name.trim() || !formData.description.trim()) {
      setError('Please fill in all fields');
      return;
    }

    setLoading(true);
    setError('');

    try {
      if (editingId) {
        const result = await productAPI.update(editingId, formData.name, formData.description);
        console.log('Update result:', result);
      } else {
        const result = await productAPI.create(
          formData.name,
          formData.description,
          parseInt(formData.initial_stock) || 50
        );
        console.log('Create result:', result);
      }

      setFormData({ name: '', description: '', initial_stock: '50' });
      setEditingId(null);
      setShowForm(false);
      await loadProducts();
    } catch (err: any) {
      console.error('Save error:', err);
      setError(err.message || 'Failed to save product');
    } finally {
      setLoading(false);
    }
  };

  const handleEdit = (product: Product) => {
    setFormData({
      name: product.name,
      description: product.description,
      initial_stock: '50',
    });
    setEditingId(product.id);
    setShowForm(true);
  };

  const handleDelete = async (id: number) => {
    if (!window.confirm('Are you sure you want to delete this product?')) {
      return;
    }

    setLoading(true);
    setError('');
    try {
      const result = await productAPI.delete(id);
      console.log('Delete result:', result);
      await loadProducts();
    } catch (err: any) {
      console.error('Delete error:', err);
      setError(err.message || 'Failed to delete product');
    } finally {
      setLoading(false);
    }
  };

  const handleUpdateStock = async (productId: number, newStock: number) => {
    try {
      await inventoryAPI.updateStock(productId, newStock);
      setInventory(prev => ({ ...prev, [productId]: newStock }));
    } catch (err) {
      setError('Failed to update stock');
    }
  };

  return (
    <div className="card">
      <h1 className="card-title">📦 Products Management</h1>

      {error && <div className="alert alert-danger">{error}</div>}

      {/* Search Bar */}
      <div className="form-group" style={{ marginBottom: '2rem' }}>
        <div style={{ display: 'flex', gap: '1rem' }}>
          <input
            type="text"
            placeholder="Search products..."
            value={searchTerm}
            onChange={(e) => setSearchTerm(e.target.value)}
            style={{ flex: 1 }}
          />
          <button className="btn btn-primary" onClick={handleSearch}>
            Search
          </button>
          <button
            className="btn btn-success"
            onClick={() => {
              setShowForm(!showForm);
              setEditingId(null);
              setFormData({ name: '', description: '', initial_stock: '50' });
            }}
          >
            {showForm ? 'Cancel' : '➕ Add Product'}
          </button>
        </div>
      </div>

      {/* Add/Edit Form */}
      {showForm && (
        <div className="card" style={{ background: '#f9f9f9', marginBottom: '2rem' }}>
          <h3>{editingId ? 'Edit Product' : 'New Product'}</h3>
          <form onSubmit={handleSubmit}>
            <div className="form-group">
              <label>Product Name</label>
              <input
                type="text"
                value={formData.name}
                onChange={(e) => setFormData({ ...formData, name: e.target.value })}
                placeholder="e.g., Laptop"
                required
              />
            </div>

            <div className="form-group">
              <label>Description</label>
              <textarea
                value={formData.description}
                onChange={(e) => setFormData({ ...formData, description: e.target.value })}
                placeholder="e.g., High-performance gaming laptop"
                required
              />
            </div>

            {!editingId && (
              <div className="form-group">
                <label>Initial Stock</label>
                <input
                  type="number"
                  value={formData.initial_stock}
                  onChange={(e) => setFormData({ ...formData, initial_stock: e.target.value })}
                  min="0"
                  required
                />
              </div>
            )}

            <button type="submit" className="btn btn-success" disabled={loading}>
              {loading ? 'Saving...' : editingId ? 'Update' : 'Create'}
            </button>
          </form>
        </div>
      )}

      {/* Products Table */}
      {loading && !products.length ? (
        <div className="loading">Loading products...</div>
      ) : products.length === 0 ? (
        <div className="empty-state">
          <div className="empty-state-icon">📭</div>
          <p>No products found</p>
        </div>
      ) : (
        <div style={{ overflowX: 'auto' }}>
          <table>
            <thead>
              <tr>
                <th>ID</th>
                <th>Name</th>
                <th>Description</th>
                <th>Stock</th>
                <th>Actions</th>
              </tr>
            </thead>
            <tbody>
              {products.map((product) => (
                <tr key={product.id}>
                  <td>#{product.id}</td>
                  <td><strong>{product.name}</strong></td>
                  <td>{product.description}</td>
                  <td>
                    <div style={{ display: 'flex', gap: '0.5rem', alignItems: 'center' }}>
                      <input
                        type="number"
                        value={inventory[product.id] || 0}
                        onChange={(e) => {
                          const newStock = parseInt(e.target.value) || 0;
                          handleUpdateStock(product.id, newStock);
                        }}
                        style={{ width: '80px', padding: '0.5rem' }}
                        min="0"
                      />
                      <span className={inventory[product.id] > 0 ? 'badge badge-success' : 'badge badge-danger'}>
                        {inventory[product.id] || 0} units
                      </span>
                    </div>
                  </td>
                  <td>
                    <div className="btn-group">
                      <button
                        className="btn btn-primary btn-small"
                        onClick={() => handleEdit(product)}
                      >
                        ✏️ Edit
                      </button>
                      <button
                        className="btn btn-danger btn-small"
                        onClick={() => handleDelete(product.id)}
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

export default ProductPage;
