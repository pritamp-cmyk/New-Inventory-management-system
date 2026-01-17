// API Service - Handles all HTTP requests to the backend

const API_BASE_URL = process.env.REACT_APP_API_BASE_URL || 'http://localhost:8080';
const API_URL = `${API_BASE_URL}/api`;

// Debug logging
console.log('🔧 API Configuration:');
console.log('   API_BASE_URL:', API_BASE_URL);
console.log('   API_URL:', API_URL);
console.log('   Environment:', process.env.REACT_APP_ENV || 'development');

// ============================================
// PRODUCTS API
// ============================================

export const productAPI = {
  // Get all products
  getAll: async () => {
    const response = await fetch(`${API_URL}/products`);
    return response.json();
  },

  // Get product by ID
  getById: async (id: number) => {
    const response = await fetch(`${API_URL}/products/${id}`);
    return response.json();
  },

  // Search products by name
  search: async (name: string) => {
    const response = await fetch(`${API_URL}/products/search?name=${encodeURIComponent(name)}`);
    return response.json();
  },

  // Create product
  create: async (name: string, description: string, initial_stock: number) => {
    try {
      const response = await fetch(`${API_URL}/products`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ name, description, initial_stock }),
      });
      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
      }
      const data = await response.json();
      console.log('✅ Product created:', data);
      return data;
    } catch (error) {
      console.error('❌ Create product failed:', error);
      throw error;
    }
  },

  // Update product
  update: async (id: number, name: string, description: string) => {
    try {
      const response = await fetch(`${API_URL}/products/${id}`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ name, description }),
      });
      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
      }
      const data = await response.json();
      console.log('✅ Product updated:', data);
      return data;
    } catch (error) {
      console.error('❌ Update product failed:', error);
      throw error;
    }
  },

  // Delete product
  delete: async (id: number) => {
    try {
      const response = await fetch(`${API_URL}/products/${id}`, {
        method: 'DELETE',
      });
      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
      }
      const data = await response.json();
      console.log('✅ Product deleted:', data);
      return data;
    } catch (error) {
      console.error('❌ Delete product failed:', error);
      throw error;
    }
  },
};

// ============================================
// INVENTORY API
// ============================================

export const inventoryAPI = {
  // Get inventory/stock level
  getStock: async (productId: number) => {
    try {
      const response = await fetch(`${API_URL}/inventory/${productId}`);
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      return response.json();
    } catch (error) {
      console.error('❌ Get stock failed:', error);
      throw error;
    }
  },

  // Update stock
  updateStock: async (productId: number, stock: number) => {
    try {
      const response = await fetch(`${API_URL}/inventory/${productId}`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ stock }),
      });
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      const data = await response.json();
      console.log('✅ Stock updated:', data);
      return data;
    } catch (error) {
      console.error('❌ Update stock failed:', error);
      throw error;
    }
  },
};

// ============================================
// USERS API
// ============================================

export const userAPI = {
  // Get all users
  getAll: async () => {
    try {
      const response = await fetch(`${API_URL}/users`);
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      return response.json();
    } catch (error) {
      console.error('❌ Get users failed:', error);
      throw error;
    }
  },

  // Get user by ID
  getById: async (id: number) => {
    try {
      const response = await fetch(`${API_URL}/users/${id}`);
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      return response.json();
    } catch (error) {
      console.error('❌ Get user failed:', error);
      throw error;
    }
  },

  // Create user
  create: async (name: string, email: string, role: 'admin' | 'user') => {
    try {
      const response = await fetch(`${API_URL}/users`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ name, email, role: role.toUpperCase() }),
      });
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      const data = await response.json();
      console.log('✅ User created:', data);
      return data;
    } catch (error) {
      console.error('❌ Create user failed:', error);
      throw error;
    }
  },

  // Update user
  update: async (id: number, name: string, email: string, role: 'admin' | 'user') => {
    try {
      const response = await fetch(`${API_URL}/users/${id}`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ name, email, role: role.toUpperCase() }),
      });
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      const data = await response.json();
      console.log('✅ User updated:', data);
      return data;
    } catch (error) {
      console.error('❌ Update user failed:', error);
      throw error;
    }
  },

  // Delete user
  delete: async (id: number) => {
    try {
      const response = await fetch(`${API_URL}/users/${id}`, {
        method: 'DELETE',
      });
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      const data = await response.json();
      console.log('✅ User deleted:', data);
      return data;
    } catch (error) {
      console.error('❌ Delete user failed:', error);
      throw error;
    }
  },
};

// ============================================
// SUBSCRIPTIONS API
// ============================================

export const subscriptionAPI = {
  // Get all subscriptions
  getAll: async () => {
    try {
      const response = await fetch(`${API_URL}/subscriptions`);
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      return response.json();
    } catch (error) {
      console.error('❌ Get subscriptions failed:', error);
      throw error;
    }
  },

  // Get subscription by ID
  getById: async (id: number) => {
    try {
      const response = await fetch(`${API_URL}/subscriptions/${id}`);
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      return response.json();
    } catch (error) {
      console.error('❌ Get subscription failed:', error);
      throw error;
    }
  },

  // Get user subscriptions
  getUserSubscriptions: async (userId: number) => {
    try {
      const response = await fetch(`${API_URL}/users/${userId}/subscriptions`);
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      return response.json();
    } catch (error) {
      console.error('❌ Get user subscriptions failed:', error);
      throw error;
    }
  },

  // Create subscription
  create: async (user_id: number, product_id: number) => {
    try {
      const response = await fetch(`${API_URL}/subscriptions`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ user_id, product_id }),
      });
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      const data = await response.json();
      console.log('✅ Subscription created:', data);
      return data;
    } catch (error) {
      console.error('❌ Create subscription failed:', error);
      throw error;
    }
  },

  // Update subscription status
  updateStatus: async (id: number, active: boolean) => {
    try {
      const response = await fetch(`${API_URL}/subscriptions/${id}`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ active }),
      });
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      const data = await response.json();
      console.log('✅ Subscription status updated:', data);
      return data;
    } catch (error) {
      console.error('❌ Update subscription status failed:', error);
      throw error;
    }
  },

  // Delete subscription
  delete: async (id: number) => {
    try {
      const response = await fetch(`${API_URL}/subscriptions/${id}`, {
        method: 'DELETE',
      });
      if (!response.ok) throw new Error(`HTTP ${response.status}`);
      const data = await response.json();
      console.log('✅ Subscription deleted:', data);
      return data;
    } catch (error) {
      console.error('❌ Delete subscription failed:', error);
      throw error;
    }
  },
};
