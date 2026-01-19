import React from 'react';
import ReactDOM from 'react-dom/client';
import App from './App';

// Diagnostic check for backend connection
const checkBackendConnection = async () => {
  const apiUrl = process.env.REACT_APP_API_BASE_URL || 'http://localhost:8080';
  console.log('🔗 Checking backend connection...');
  console.log('   Target URL:', apiUrl);
  
  try {
    const response = await fetch(`${apiUrl}/api/products`);
    console.log(' Backend connection successful!');
    console.log('   Status:', response.status);
  } catch (error) {
    console.error('Backend connection failed!');
    console.error('   Error:', error);
  }
};

// Check connection on app load
checkBackendConnection();

const root = ReactDOM.createRoot(
  document.getElementById('root') as HTMLElement
);
root.render(
  <React.StrictMode>
    <App />
  </React.StrictMode>
);
