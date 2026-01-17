# 🚀 Developer Quick Reference

## 📋 Frontend File Map

```
src/
├── App.tsx                          # Main app with navigation
├── App.css                          # All styling (500+ lines)
├── index.tsx                        # React entry point
├── pages/
│   ├── DashboardPage.tsx           # Statistics & overview
│   ├── ProductPage.tsx             # Product management
│   ├── UserPage.tsx                # User management
│   └── SubscriptionPage.tsx        # Subscription management
└── services/
    └── api.ts                      # All API calls
```

## ⚡ Common Tasks

### Add a New API Endpoint

**Step 1**: Add to [frontend/src/services/api.ts](frontend/src/services/api.ts)

```typescript
// Example: Get product by name
productAPI: {
  getByName: async (name: string) => {
    const response = await axios.get(`${API_BASE_URL}/api/products/name/${name}`);
    return response.data;
  }
}
```

**Step 2**: Use in component

```typescript
const handleSearch = async (name: string) => {
  try {
    const product = await productAPI.getByName(name);
    setProduct(product);
  } catch (err) {
    setError('Not found');
  }
};
```

### Change API Base URL

Edit [frontend/src/services/api.ts](frontend/src/services/api.ts) line 3:

```typescript
const API_BASE_URL = 'http://localhost:8080'; // Change this
```

### Add New Page

1. Create file: `src/pages/NewPage.tsx`
2. Copy pattern from ProductPage.tsx
3. Add API calls to services/api.ts
4. Import in App.tsx
5. Add navigation button
6. Add routing logic

### Style a Component

Add to [frontend/src/App.css](frontend/src/App.css):

```css
.my-component {
  background: white;
  padding: 1rem;
  border-radius: 8px;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
}

.my-component:hover {
  transform: translateY(-2px);
}
```

Then use in JSX:

```tsx
<div className="my-component">Content</div>
```

### Handle Errors

```typescript
try {
  const data = await productAPI.getAll();
  setData(data);
} catch (err) {
  setError('Failed to load products');
} finally {
  setLoading(false);
}
```

### Show Loading State

```typescript
{loading && <div className="loading">Loading...</div>}
{!loading && data.length === 0 && <div className="empty-state">No data</div>}
{!loading && data.length > 0 && <table>...</table>}
```

## 🔑 Key Patterns

### State Management
```typescript
const [data, setData] = useState<Type[]>([]);
const [loading, setLoading] = useState(false);
const [error, setError] = useState('');
const [form, setForm] = useState({ /* fields */ });
```

### Data Loading
```typescript
useEffect(() => {
  loadData();
}, []); // Run once on mount

const loadData = async () => {
  setLoading(true);
  try {
    const result = await api.call();
    setData(result);
  } catch (err) {
    setError('Error message');
  } finally {
    setLoading(false);
  }
};
```

### Form Handling
```typescript
const handleSubmit = async (e: React.FormEvent) => {
  e.preventDefault();
  try {
    await api.create(form);
    setForm({ /* reset */ });
    loadData(); // Refresh
  } catch (err) {
    setError('Failed');
  }
};
```

### CRUD Operations
```typescript
// Create
await productAPI.create({ name, price });

// Read
await productAPI.getAll();
await productAPI.getById(id);

// Update
await productAPI.update(id, { name, price });

// Delete
await productAPI.delete(id);
```

## 🎯 Navigation

```typescript
// In App.tsx
const [currentPage, setCurrentPage] = useState<Page>('dashboard');

// In navbar
<button onClick={() => setCurrentPage('products')}>Products</button>

// In main
{currentPage === 'products' && <ProductPage />}
```

## 🧪 Testing with Curl

### Backend Verification
```bash
curl http://localhost:8080/api/products
curl http://localhost:8080/api/users
curl http://localhost:8080/api/subscriptions
```

### Create Record
```bash
curl -X POST http://localhost:8080/api/products \
  -H "Content-Type: application/json" \
  -d '{"name":"Test","description":"Desc","price":99.99}'
```

### Update Record
```bash
curl -X PUT http://localhost:8080/api/products/1 \
  -H "Content-Type: application/json" \
  -d '{"name":"Updated","price":199.99}'
```

### Delete Record
```bash
curl -X DELETE http://localhost:8080/api/products/1
```

## 🔧 Development Commands

```bash
# Install dependencies
npm install

# Start development server
npm start

# Build for production
npm run build

# Run tests
npm test

# Check for errors
npm run tsc --noEmit
```

## 📱 Responsive Design

Mobile breakpoint at 768px:
```css
@media (max-width: 768px) {
  /* Mobile styles */
}
```

Common adjustments:
- Single column layout
- Larger touch targets
- Simplified forms
- Reduced spacing

## 🐛 Debugging

### Browser Console (F12)
- Check network requests
- Review error messages
- Inspect component state
- Monitor API calls

### React Developer Tools
- Inspect component tree
- Check props and state
- Track re-renders
- Profile performance

### VS Code
- Set breakpoints
- Debug sessions
- Hover for types
- IntelliSense completion

## 💾 File Size Reference

| File | Lines | Size |
|------|-------|------|
| App.tsx | 56 | ~2KB |
| App.css | 550 | ~15KB |
| ProductPage.tsx | 280 | ~8KB |
| UserPage.tsx | 210 | ~6KB |
| SubscriptionPage.tsx | 210 | ~6KB |
| DashboardPage.tsx | 290 | ~9KB |
| api.ts | 200 | ~6KB |
| **Total** | **~2000** | **~60KB** |

## 📊 Component Map

```
App (56 lines)
├── Navbar
│   ├── Dashboard link
│   ├── Products link
│   ├── Users link
│   └── Subscriptions link
└── Main Content (4 page components)
    ├── DashboardPage (290 lines)
    │   ├── Stats Cards
    │   ├── Product Preview
    │   ├── Low Stock Alerts
    │   └── Quick Actions
    ├── ProductPage (280 lines)
    │   ├── Search Form
    │   ├── Create Form
    │   ├── Products Table
    │   └── Edit/Delete
    ├── UserPage (210 lines)
    │   ├── Create Form
    │   ├── Users Table
    │   └── Edit/Delete
    └── SubscriptionPage (210 lines)
        ├── Create Form
        ├── Subscriptions Table
        └── Pause/Resume/Delete
```

## 🎨 CSS Class Reference

### Layout
- `.App` - Main container
- `.navbar` - Top navigation
- `.main-content` - Page content area
- `.card` - Content wrapper

### Components
- `.btn`, `.btn-success`, `.btn-danger` - Buttons
- `.table` - Data tables
- `.form-group` - Form sections
- `.badge` - Status badges

### States
- `.loading` - Loading indicator
- `.error`, `.alert-danger` - Error messages
- `.success`, `.alert-success` - Success messages
- `.empty-state` - Empty data state

### Utilities
- `.grid` - Grid layout
- `.btn-group` - Button grouping
- `.btn-small` - Small buttons
- `.btn-lg` - Large buttons

## 📝 Environment Variables

### .env
```
REACT_APP_API_BASE_URL=http://localhost:8080
REACT_APP_ENV=development
```

### Usage
```typescript
const apiUrl = process.env.REACT_APP_API_BASE_URL;
```

## 🚀 Performance Tips

1. **Lazy load components** - Import only when needed
2. **Memoize callbacks** - Use useCallback for handlers
3. **Debounce search** - Reduce API calls
4. **Virtual scrolling** - For large lists
5. **Code splitting** - Separate route bundles
6. **Image optimization** - Compress and resize
7. **CSS optimization** - Remove unused styles
8. **API caching** - Cache repeated requests

## 🎓 Code Review Checklist

- [ ] No console errors
- [ ] Proper error handling
- [ ] Loading states present
- [ ] Mobile responsive
- [ ] Type-safe (no `any`)
- [ ] Consistent styling
- [ ] Accessible HTML
- [ ] No unused imports
- [ ] Comments for complex logic
- [ ] Proper indentation

## 📚 Additional Resources

- [React Docs](https://react.dev)
- [TypeScript Handbook](https://www.typescriptlang.org/docs/)
- [MDN Web Docs](https://developer.mozilla.org/)
- [CSS-Tricks](https://css-tricks.com)
- [Axios Docs](https://axios-http.com)

## 🔗 Related Files

- **FRONTEND_GUIDE.md** - Detailed development guide
- **INTEGRATION_GUIDE.md** - Full system setup
- **COMPLETE_DOCUMENTATION.md** - Complete project docs
- **frontend/README.md** - Frontend-specific info

---

**Last Updated**: January 2024
**Framework**: React 18 + TypeScript
**Status**: Production Ready ✅
