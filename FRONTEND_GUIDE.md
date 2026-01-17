# Frontend Development Guide

## 📚 Overview

This guide teaches you how the React TypeScript frontend is built for the Inventory Management System.

## 🎯 Architecture Pattern

The frontend follows a clean architecture with separation of concerns:

```
┌─────────────────────────────────────┐
│  React Components (Pages)           │
│  - ProductPage                      │
│  - UserPage                         │
│  - SubscriptionPage                 │
│  - DashboardPage                    │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│  API Service Layer (services/api.ts)│
│  - productAPI                       │
│  - userAPI                          │
│  - subscriptionAPI                  │
│  - inventoryAPI                     │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│  Backend REST API                   │
│  (C++ Server on :8080)              │
└─────────────────────────────────────┘
```

## 🔧 Core Components

### 1. API Service (`src/services/api.ts`)

**Purpose**: Centralized API calls with TypeScript typing

```typescript
// Example: Fetching all products
const productAPI = {
  getAll: async () => {
    const response = await axios.get(`${API_BASE_URL}/api/products`);
    return response.data;
  }
};
```

**Benefits**:
- Single source of truth for all API endpoints
- Type-safe requests and responses
- Easy to modify endpoints globally
- Centralized error handling

### 2. Page Components

**Pattern Used**: React Functional Components with Hooks

```typescript
const ProductPage: React.FC = () => {
  // State management
  const [products, setProducts] = useState<Product[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  
  // Data fetching
  useEffect(() => {
    loadProducts();
  }, []);
  
  // Event handlers
  const handleCreate = async (data) => { ... };
  const handleUpdate = async (id, data) => { ... };
  const handleDelete = async (id) => { ... };
  
  // Render
  return (
    <div>
      {/* JSX rendering */}
    </div>
  );
};
```

### 3. Styling System (`src/App.css`)

**Approach**: Single CSS file with BEM-like naming

```css
.navbar { ... }           /* Component block */
.navbar-brand { ... }     /* Element */
.navbar.active { ... }    /* Modifier */

.card { ... }
.card-title { ... }
.card-content { ... }
```

## 🚀 How to Build a New Page

### Step 1: Create the TypeScript File

```bash
touch src/pages/NewPage.tsx
```

### Step 2: Define Types

```typescript
interface Item {
  id: number;
  name: string;
  // ... other properties
}
```

### Step 3: Create Component Structure

```typescript
const NewPage: React.FC = () => {
  // State hooks
  const [items, setItems] = useState<Item[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const [showForm, setShowForm] = useState(false);

  // Load data on mount
  useEffect(() => {
    loadItems();
  }, []);

  // API call handler
  const loadItems = async () => {
    setLoading(true);
    setError('');
    try {
      const data = await itemAPI.getAll();
      setItems(Array.isArray(data) ? data : []);
    } catch (err) {
      setError('Failed to load items');
    } finally {
      setLoading(false);
    }
  };

  // CRUD handlers
  const handleCreate = async (formData) => { /* ... */ };
  const handleUpdate = async (id, formData) => { /* ... */ };
  const handleDelete = async (id) => { /* ... */ };

  // Render
  return (
    <div className="card">
      {/* Content */}
    </div>
  );
};
```

### Step 4: Add to App.tsx

```typescript
import NewPage from './pages/NewPage';

type Page = 'dashboard' | 'products' | 'users' | 'subscriptions' | 'new';

function App() {
  const [currentPage, setCurrentPage] = useState<Page>('dashboard');

  return (
    <>
      <nav className="navbar">
        {/* Add button */}
        <button onClick={() => setCurrentPage('new')}>New Page</button>
      </nav>
      <main>
        {currentPage === 'new' && <NewPage />}
      </main>
    </>
  );
}
```

## 📋 Page Component Walkthrough

### ProductPage Example

**Structure**:
```
ProductPage
├── State
│   ├── products (array of Product)
│   ├── loading (boolean)
│   ├── error (string)
│   └── showForm (boolean)
├── Effects
│   └── Load data on mount
├── Handlers
│   ├── loadProducts()
│   ├── handleSubmit()
│   ├── handleUpdate()
│   ├── handleDelete()
│   └── handleSearch()
└── Render
    ├── Header
    ├── Create/Search Form
    ├── Products Table
    └── Error/Loading States
```

**Key Features Demonstrated**:

1. **Async Data Loading**
```typescript
useEffect(() => {
  loadData();
}, []);

const loadData = async () => {
  setLoading(true);
  try {
    const data = await productAPI.getAll();
    setProducts(data);
  } catch (err) {
    setError('Failed to load products');
  } finally {
    setLoading(false);
  }
};
```

2. **Form Handling**
```typescript
const handleSubmit = async (e: React.FormEvent) => {
  e.preventDefault();
  try {
    await productAPI.create(formData);
    setFormData({ /* reset */ });
    loadData(); // Refresh
  } catch (err) {
    setError('Failed to create');
  }
};
```

3. **Conditional Rendering**
```typescript
{loading && <div className="loading">Loading...</div>}
{error && <div className="alert alert-danger">{error}</div>}
{products.length === 0 && <div className="empty-state">No items</div>}
{products.length > 0 && <table>/* ... */</table>}
```

4. **Search Functionality**
```typescript
const handleSearch = async (query: string) => {
  if (!query) {
    loadData();
    return;
  }
  setLoading(true);
  try {
    const results = await productAPI.search(query);
    setProducts(results);
  } catch (err) {
    setError('Search failed');
  } finally {
    setLoading(false);
  }
};
```

## 🎨 CSS Patterns

### Card Component
```css
.card {
  background: white;
  padding: 2rem;
  border-radius: 8px;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
  margin-bottom: 2rem;
}

.card-title {
  color: #2c3e50;
  font-size: 1.8rem;
  margin-bottom: 1.5rem;
  border-bottom: 2px solid #667eea;
  padding-bottom: 0.5rem;
}
```

### Button Variations
```css
.btn {
  padding: 0.75rem 1.5rem;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  transition: all 0.2s ease;
  font-size: 0.95rem;
  font-weight: 600;
}

.btn-success { background: #2ecc71; color: white; }
.btn-danger { background: #e74c3c; color: white; }
.btn-primary { background: #3498db; color: white; }

.btn:hover { transform: translateY(-2px); box-shadow: 0 4px 8px rgba(0,0,0,0.2); }
```

### Table Styling
```css
table {
  width: 100%;
  border-collapse: collapse;
  background: white;
}

th {
  background: #ecf0f1;
  padding: 1rem;
  text-align: left;
  font-weight: 600;
  color: #2c3e50;
}

td {
  padding: 1rem;
  border-bottom: 1px solid #ecf0f1;
}

tr:hover { background: #f9f9f9; }
```

## 🔄 Data Flow Example

**Scenario**: Creating a new product

```
1. User fills form
   ↓
2. handleSubmit() called
   ↓
3. setLoading(true) - Show loading state
   ↓
4. productAPI.create(formData) - API call
   ↓
5. Backend processes request (:8080)
   ↓
6. API returns response
   ↓
7. setFormData({}) - Clear form
   ↓
8. setLoading(false) - Hide loading
   ↓
9. loadData() - Refresh product list
   ↓
10. Success: Products table updates
```

## 🛠️ Common Patterns

### Pattern 1: Parallel Data Fetching
```typescript
const [data1, data2, data3] = await Promise.all([
  api1.getAll(),
  api2.getAll(),
  api3.getAll(),
]);
```

### Pattern 2: Error Handling
```typescript
try {
  // API call
} catch (err) {
  setError('User-friendly message');
} finally {
  setLoading(false);
}
```

### Pattern 3: Confirmation Dialog
```typescript
const handleDelete = async (id: number) => {
  if (!window.confirm('Are you sure?')) return;
  // Proceed with delete
};
```

### Pattern 4: Inline Editing
```typescript
{isEditing ? (
  <input value={editValue} onChange={(e) => setEditValue(e.target.value)} />
) : (
  <span onClick={() => setIsEditing(true)}>{value}</span>
)}
```

## 📊 State Management Tips

1. **Keep state flat** - Avoid nested objects when possible
2. **Use multiple states** - Separate concerns (data, loading, error, form)
3. **Lift state up** - If siblings need shared state, move to parent
4. **Use useCallback** - Prevent unnecessary re-renders of child components
5. **Avoid state in loops** - Use keys in map() for lists

## 🚨 Common Pitfalls

1. **Missing dependencies in useEffect**
```typescript
// ❌ Wrong - runs on every render
useEffect(() => { loadData(); });

// ✅ Correct - runs once on mount
useEffect(() => { loadData(); }, []);
```

2. **Forgetting to handle array responses**
```typescript
// ❌ Wrong - crashes if data is null
const data = await api.getAll();
setItems(data);

// ✅ Correct - safe fallback
const data = await api.getAll();
setItems(Array.isArray(data) ? data : []);
```

3. **Not clearing loading state**
```typescript
// ❌ Wrong - stays loading forever if error
try {
  await api.call();
} catch (err) {
  setError(err);
}

// ✅ Correct - always clear loading
finally {
  setLoading(false);
}
```

## 📱 Responsive Design

The CSS includes mobile breakpoints:

```css
@media (max-width: 768px) {
  .grid {
    grid-template-columns: 1fr;
  }
  
  .navbar {
    flex-direction: column;
  }
  
  table {
    font-size: 0.9rem;
  }
}
```

## 🎓 Learning Path

1. **Understand the architecture** - API Service → Components → Pages
2. **Learn React hooks** - useState, useEffect, useCallback
3. **Study the API service** - How endpoints are wrapped
4. **Review ProductPage** - Most complete example
5. **Build a new page** - Practice the pattern
6. **Master styling** - CSS Grid, Flexbox, Responsive design
7. **Handle edge cases** - Errors, loading, empty states

## 🔗 File Dependencies

```
App.tsx (imports)
├── ProductPage.tsx
├── UserPage.tsx
├── SubscriptionPage.tsx
├── DashboardPage.tsx
└── App.css

All pages import:
└── services/api.ts
    └── axios
```

## 📈 Next Steps

To enhance the frontend:

1. Add authentication (login/logout)
2. Add toast notifications
3. Add pagination
4. Add sorting/filtering
5. Add export to CSV
6. Add charts/graphs
7. Add role-based access
8. Add offline support
9. Add unit tests
10. Deploy to production

---

**Happy Coding!** 🚀
