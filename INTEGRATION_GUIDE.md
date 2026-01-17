# Complete Inventory System - Setup & Integration Guide

## 📦 Project Overview

This is a full-stack inventory management system with:
- **Backend**: C++ REST API with PostgreSQL
- **Frontend**: React TypeScript SPA
- **Database**: PostgreSQL

## 🏗️ Architecture

```
┌─────────────────────────────────────────┐
│         React Frontend (:3000)          │
│  - Dashboard, Products, Users, Subs    │
└──────────────┬──────────────────────────┘
               │ HTTP Requests
               │ (Axios)
┌──────────────▼──────────────────────────┐
│    C++ Backend REST API (:8080)         │
│  - 19 Endpoints (CRUD Operations)      │
└──────────────┬──────────────────────────┘
               │ SQL Queries
               │ (libpqxx)
┌──────────────▼──────────────────────────┐
│       PostgreSQL Database               │
│  - Products, Users, Subscriptions      │
└─────────────────────────────────────────┘
```

## 🚀 Complete Setup Instructions

### Phase 1: Backend Setup (Already Complete ✓)

Your C++ backend is ready on `http://localhost:8080`

**Verify Backend:**
```bash
cd /Users/pritam/inventory-project
# Start the server (from previous session)
./src/main
```

### Phase 2: Frontend Setup

#### Step 1: Install Node.js Dependencies
```bash
cd /Users/pritam/inventory-project/frontend
npm install
```

This installs:
- React & React-DOM
- TypeScript
- Axios (HTTP client)
- React-scripts (build tools)

#### Step 2: Create .env File
```bash
cp .env.example .env
```

**Content of `.env`:**
```
REACT_APP_API_BASE_URL=http://localhost:8080
REACT_APP_ENV=development
```

#### Step 3: Start Development Server
```bash
npm start
```

This will:
- Start React dev server on `http://localhost:3000`
- Open browser automatically
- Enable hot-reload for file changes

### Phase 3: Verify Integration

#### Test 1: Dashboard Page
- Navigate to http://localhost:3000
- Should see statistics loading
- Should display product count, user count, etc.

#### Test 2: Products Page
- Click "Products" in navbar
- Should see all products in table
- Try creating a new product
- Verify it appears in list

#### Test 3: Users Page
- Click "Users" in navbar
- Should see all system users
- Try creating a new user
- Verify user appears in list

#### Test 4: Subscriptions Page
- Click "Subscriptions" in navbar
- Should see all subscriptions
- Try creating new subscription
- Verify subscription appears

## 📊 API Endpoints Reference

### Products
| Method | Endpoint | Purpose |
|--------|----------|---------|
| GET | `/api/products` | Get all products |
| GET | `/api/products/:id` | Get specific product |
| GET | `/api/products/search?q=` | Search products |
| POST | `/api/products` | Create product |
| PUT | `/api/products/:id` | Update product |
| DELETE | `/api/products/:id` | Delete product |

### Users
| Method | Endpoint | Purpose |
|--------|----------|---------|
| GET | `/api/users` | Get all users |
| GET | `/api/users/:id` | Get specific user |
| POST | `/api/users` | Create user |
| PUT | `/api/users/:id` | Update user |
| DELETE | `/api/users/:id` | Delete user |

### Subscriptions
| Method | Endpoint | Purpose |
|--------|----------|---------|
| GET | `/api/subscriptions` | Get all subscriptions |
| GET | `/api/subscriptions/:id` | Get specific subscription |
| GET | `/api/users/:userId/subscriptions` | Get user subscriptions |
| POST | `/api/subscriptions` | Create subscription |
| PUT | `/api/subscriptions/:id` | Update subscription |
| DELETE | `/api/subscriptions/:id` | Delete subscription |

### Inventory
| Method | Endpoint | Purpose |
|--------|----------|---------|
| GET | `/api/inventory/:productId` | Get stock |
| PUT | `/api/inventory/:productId` | Update stock |

## 🔧 Configuration Files

### Frontend Configuration

**`frontend/package.json`** - Dependencies & scripts
```json
{
  "scripts": {
    "start": "react-scripts start",      // Dev server
    "build": "react-scripts build",      // Production build
    "test": "react-scripts test"         // Run tests
  }
}
```

**`frontend/tsconfig.json`** - TypeScript settings
- Target: ES2020
- Strict mode enabled
- Module resolution: bundler

**`frontend/.env`** - Environment variables
- API base URL configuration

### Backend Configuration

**`src/main.cpp`** - Server runs on port 8080
**`db/init.sql`** - Database initialization script
**`docker-compose.yml`** - PostgreSQL container setup

## 🗄️ Database Setup

If database isn't running:

```bash
# Start PostgreSQL container
docker-compose up -d

# Initialize database
docker exec -i inventory-db psql -U postgres < db/init.sql
```

## 📁 Project Structure

```
inventory-project/
├── src/                          # C++ Backend
│   ├── main.cpp
│   ├── controller/               # API Routes
│   │   ├── ProductRoutes.h
│   │   ├── UserRoutes.h
│   │   └── SubscriptionRoutes.h
│   ├── service/                  # Business Logic
│   ├── repository/               # Database Access
│   └── domain/                   # Data Models
├── frontend/                     # React Frontend
│   ├── public/
│   │   └── index.html
│   ├── src/
│   │   ├── pages/
│   │   │   ├── DashboardPage.tsx
│   │   │   ├── ProductPage.tsx
│   │   │   ├── UserPage.tsx
│   │   │   └── SubscriptionPage.tsx
│   │   ├── services/
│   │   │   └── api.ts
│   │   ├── App.tsx
│   │   ├── App.css
│   │   └── index.tsx
│   ├── package.json
│   └── tsconfig.json
├── db/                           # Database
│   └── init.sql
├── docker-compose.yml
└── Makefile                      # Build scripts
```

## 🧪 Testing the Full System

### Quick Integration Test

```bash
# Terminal 1: Start Backend
cd /Users/pritam/inventory-project
./src/main

# Terminal 2: Start Frontend
cd /Users/pritam/inventory-project/frontend
npm start
```

### Manual Test Sequence

1. **Create a Product**
   - Go to Products page
   - Click "➕ New Product"
   - Fill form: Name, Description, Price
   - Click "Create Product"
   - Verify it appears in table

2. **Create a User**
   - Go to Users page
   - Click "➕ New User"
   - Fill form: Name, Email, Role
   - Click "Create User"
   - Verify it appears in table

3. **Create a Subscription**
   - Go to Subscriptions page
   - Click "➕ New Subscription"
   - Select user and product
   - Click "Create Subscription"
   - Verify it appears in table

4. **Check Dashboard**
   - Go to Dashboard
   - See updated statistics
   - See recent products
   - See low stock alerts

5. **Update Product Stock**
   - Go to Products page
   - Click "📦 Update Stock"
   - Change quantity
   - Verify Dashboard updates

## 🐛 Troubleshooting

### Frontend Won't Start
```bash
# Clear cache and reinstall
rm -rf node_modules package-lock.json
npm install
npm start
```

### Backend Connection Error
- Verify backend is running: `curl http://localhost:8080/api/products`
- Check `.env` has correct API URL
- Check browser console (F12) for CORS errors

### Database Connection Error
- Start Docker: `docker-compose up -d`
- Initialize DB: `docker exec -i inventory-db psql -U postgres < db/init.sql`
- Check PostgreSQL is running: `docker ps`

### CORS Issues
If you get CORS errors, the backend needs these headers:

```cpp
res.set_header("Access-Control-Allow-Origin", "*");
res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE");
res.set_header("Access-Control-Allow-Headers", "Content-Type");
```

## 📈 Production Deployment

### Build Frontend
```bash
cd frontend
npm run build
```

Creates optimized `build/` directory (~150KB gzipped)

### Deploy Options

**Option 1: Static Hosting (Netlify, Vercel)**
- Deploy `build/` folder
- Configure environment variables

**Option 2: Docker**
```dockerfile
FROM node:18
WORKDIR /app
COPY . .
RUN npm install && npm run build
EXPOSE 3000
CMD ["npm", "start"]
```

**Option 3: Traditional Server**
- Copy `build/` to web server (nginx, Apache)
- Configure reverse proxy to backend

## 📚 File Reference

### Key Frontend Files

| File | Purpose |
|------|---------|
| [frontend/src/services/api.ts](frontend/src/services/api.ts) | All API endpoints |
| [frontend/src/App.tsx](frontend/src/App.tsx) | Main app routing |
| [frontend/src/App.css](frontend/src/App.css) | Global styles |
| [frontend/src/pages/DashboardPage.tsx](frontend/src/pages/DashboardPage.tsx) | Statistics dashboard |
| [frontend/src/pages/ProductPage.tsx](frontend/src/pages/ProductPage.tsx) | Product CRUD |
| [frontend/src/pages/UserPage.tsx](frontend/src/pages/UserPage.tsx) | User CRUD |
| [frontend/src/pages/SubscriptionPage.tsx](frontend/src/pages/SubscriptionPage.tsx) | Subscription CRUD |

### Backend Files

| File | Purpose |
|------|---------|
| [src/main.cpp](src/main.cpp) | Server entry point |
| [src/controller/ProductRoutes.h](src/controller/ProductRoutes.h) | Product endpoints |
| [src/repository/postgres/ProductRepo.cpp](src/repository/postgres/ProductRepo.cpp) | Database access |

## ✅ Verification Checklist

- [ ] Backend compiles and runs
- [ ] Database is initialized
- [ ] Frontend dependencies installed (`npm install`)
- [ ] Frontend starts without errors (`npm start`)
- [ ] Can see Dashboard with statistics
- [ ] Can create products
- [ ] Can create users
- [ ] Can create subscriptions
- [ ] Can update product inventory
- [ ] All pages responsive on mobile

## 🎓 Key Technologies

| Component | Technology | Version |
|-----------|-----------|---------|
| Frontend | React | 18.x |
| Frontend | TypeScript | 5.x |
| Frontend | Axios | 1.x |
| Backend | C++ | C++17 |
| Backend | cpp-httplib | Latest |
| Backend | libpqxx | 7.x |
| Database | PostgreSQL | 13+ |

## 🤝 Next Steps

1. **Test the full integration** - Verify all CRUD operations work
2. **Add error recovery** - Implement retry logic
3. **Add authentication** - Implement login system
4. **Add pagination** - For large datasets
5. **Add sorting** - In tables
6. **Add filtering** - Advanced search
7. **Add notifications** - Toast/alerts
8. **Add unit tests** - Frontend & backend
9. **Setup CI/CD** - Automated testing
10. **Deploy** - To production

## 📞 Support

- Check [FRONTEND_GUIDE.md](FRONTEND_GUIDE.md) for detailed frontend documentation
- Check backend README for C++ setup details
- Review API test files for endpoint examples

---

**System Ready!** You now have a complete, modern full-stack inventory management system. 🎉
