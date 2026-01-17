# Inventory Management System - Frontend

A modern React TypeScript frontend for the Inventory Management System with complete CRUD operations for Products, Users, and Subscriptions.

## 📋 Features

- **Dashboard**: Overview of system statistics, recent products, and low stock alerts
- **Products Management**: Create, read, update, delete products with real-time inventory tracking
- **Users Management**: Manage system users with role-based access (admin/user)
- **Subscriptions Management**: Create and manage product subscriptions for users
- **Real-time Updates**: Automatic data refresh and status updates
- **Error Handling**: Comprehensive error messages and user feedback
- **Loading States**: Visual feedback during data operations
- **Responsive Design**: Works seamlessly on desktop and mobile devices

## 🛠️ Technology Stack

- **React 18**: UI library
- **TypeScript**: Type-safe JavaScript
- **CSS 3**: Responsive styling with gradients and modern features
- **Axios**: HTTP client for API calls

## 📁 Project Structure

```
frontend/
├── public/
│   └── index.html           # HTML entry point
├── src/
│   ├── pages/
│   │   ├── DashboardPage.tsx       # Overview and statistics
│   │   ├── ProductPage.tsx         # Product CRUD operations
│   │   ├── UserPage.tsx            # User CRUD operations
│   │   └── SubscriptionPage.tsx    # Subscription management
│   ├── services/
│   │   └── api.ts           # API service layer with TypeScript types
│   ├── App.tsx              # Main app component with routing
│   ├── App.css              # Global styles
│   └── index.tsx            # React root entry point
├── package.json             # Dependencies and scripts
└── tsconfig.json            # TypeScript configuration
```

## 🚀 Getting Started

### Prerequisites

- Node.js (v16 or higher)
- npm or yarn
- Backend server running on `http://localhost:8080`

### Installation

1. Navigate to the frontend directory:
```bash
cd frontend
```

2. Install dependencies:
```bash
npm install
```

3. Start the development server:
```bash
npm start
```

The application will open at `http://localhost:3000`

## 📝 Configuration

The API base URL is configured in [src/services/api.ts](src/services/api.ts):

```typescript
const API_BASE_URL = 'http://localhost:8080';
```

If your backend runs on a different port, update this constant.

## 🔌 API Integration

The frontend integrates with the following backend endpoints:

### Products API
- `GET /api/products` - Get all products
- `GET /api/products/:id` - Get product by ID
- `GET /api/products/search?query=` - Search products
- `POST /api/products` - Create product
- `PUT /api/products/:id` - Update product
- `DELETE /api/products/:id` - Delete product

### Inventory API
- `GET /api/inventory/:productId` - Get stock for product
- `PUT /api/inventory/:productId` - Update stock quantity

### Users API
- `GET /api/users` - Get all users
- `GET /api/users/:id` - Get user by ID
- `POST /api/users` - Create user
- `PUT /api/users/:id` - Update user
- `DELETE /api/users/:id` - Delete user

### Subscriptions API
- `GET /api/subscriptions` - Get all subscriptions
- `GET /api/subscriptions/:id` - Get subscription by ID
- `GET /api/users/:userId/subscriptions` - Get user's subscriptions
- `POST /api/subscriptions` - Create subscription
- `PUT /api/subscriptions/:id` - Update subscription status
- `DELETE /api/subscriptions/:id` - Delete subscription

## 📊 Page Components

### Dashboard Page
Displays system overview:
- Total products, users, and subscriptions counts
- Active subscriptions count
- Low stock products count
- Recent products preview
- Low stock alerts table
- Quick action buttons

### Products Page
Full product management:
- View all products in a table
- Search products by name
- Create new products with name, description, and price
- Edit product details
- Delete products
- Real-time stock management
- Visual stock status indicators

### Users Page
Complete user management:
- View all system users
- Create new users with email and name
- Assign roles (admin/user)
- Edit user information
- Delete users
- Role badges for quick identification

### Subscriptions Page
Subscription management:
- View all active and inactive subscriptions
- See user and product details for each subscription
- Create new subscriptions (select user + product)
- Pause/Resume subscriptions
- Delete subscriptions
- Status indicators (Active/Inactive)

## 🎨 Styling

The application features a modern design with:
- Purple gradient background
- Clean white cards with shadows
- Color-coded badges for status
- Responsive grid layouts
- Smooth transitions and hover effects
- Mobile-friendly breakpoints

### CSS Variables & Colors
- Primary: `#667eea` (Purple)
- Secondary: `#764ba2` (Dark Purple)
- Success: `#2ecc71` (Green)
- Danger: `#e74c3c` (Red)
- Warning: `#f39c12` (Orange)
- Info: `#3498db` (Blue)

## 🔧 Build for Production

```bash
npm run build
```

This creates an optimized production build in the `build/` directory.

## 📱 Features in Detail

### Search Functionality
- Real-time product search
- Debounced API calls to reduce server load
- Instant results display

### Stock Management
- Inline stock editing on ProductPage
- Visual indicators for stock levels
- Low stock alerts on Dashboard

### Error Handling
- User-friendly error messages
- Network error recovery
- Form validation
- Loading states during operations

### State Management
- React hooks (useState, useEffect)
- Promise.all for parallel data fetching
- Optimistic UI updates

## 🐛 Troubleshooting

### CORS Issues
If you get CORS errors, ensure your backend has CORS headers enabled:

```cpp
// Add to C++ backend
res.set_header("Access-Control-Allow-Origin", "*");
res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
res.set_header("Access-Control-Allow-Headers", "Content-Type");
```

### API Connection Issues
- Verify backend is running on `localhost:8080`
- Check browser console for error messages
- Ensure firewall allows local traffic

### Styling Issues
- Clear browser cache (Ctrl+Shift+Delete or Cmd+Shift+Delete)
- Hard refresh the page (Ctrl+Shift+R or Cmd+Shift+R)

## 📚 Learning Resources

- [React Documentation](https://react.dev)
- [TypeScript Handbook](https://www.typescriptlang.org/docs/)
- [Axios Documentation](https://axios-http.com)

## 🤝 Contributing

To add new features:

1. Create a new page component in `src/pages/`
2. Add API calls to `src/services/api.ts`
3. Add navigation in `src/App.tsx`
4. Style using classes in `src/App.css`

## 📄 License

This project is part of the Inventory Management System. All rights reserved.
