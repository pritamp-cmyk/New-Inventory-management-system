import React, { useState } from 'react';
import './App.css';
import ProductPage from './pages/ProductPage';
import UserPage from './pages/UserPage';
import SubscriptionPage from './pages/SubscriptionPage';
import DashboardPage from './pages/DashboardPage';

type Page = 'dashboard' | 'products' | 'users' | 'subscriptions';

function App() {
  const [currentPage, setCurrentPage] = useState<Page>('dashboard');

  return (
    <div className="App">
      <nav className="navbar">
        <div className="navbar-brand">📦 Inventory Management</div>
        <div className="nav-links">
          <button 
            className={`nav-link ${currentPage === 'dashboard' ? 'active' : ''}`}
            onClick={() => setCurrentPage('dashboard')}
          >
            Dashboard
          </button>
          <button 
            className={`nav-link ${currentPage === 'products' ? 'active' : ''}`}
            onClick={() => setCurrentPage('products')}
          >
            Products
          </button>
          <button 
            className={`nav-link ${currentPage === 'users' ? 'active' : ''}`}
            onClick={() => setCurrentPage('users')}
          >
            Users
          </button>
          <button 
            className={`nav-link ${currentPage === 'subscriptions' ? 'active' : ''}`}
            onClick={() => setCurrentPage('subscriptions')}
          >
            Subscriptions
          </button>
        </div>
      </nav>

      <main className="main-content">
        {currentPage === 'dashboard' && <DashboardPage />}
        {currentPage === 'products' && <ProductPage />}
        {currentPage === 'users' && <UserPage />}
        {currentPage === 'subscriptions' && <SubscriptionPage />}
      </main>
    </div>
  );
}

export default App;
