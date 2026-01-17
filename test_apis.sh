#!/bin/bash

# Inventory Management API - cURL Test Suite
# Base URL
BASE_URL="http://localhost:8080"

echo "========================================="
echo "Inventory Management API - Test Suite"
echo "========================================="
echo ""

# Color codes for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# ==========================================
# PRODUCTS API TESTS
# ==========================================
echo -e "${BLUE}=== PRODUCTS API ===${NC}\n"

# 1. Get all products
echo -e "${YELLOW}1. Get All Products${NC}"
curl -X GET "$BASE_URL/api/products" \
  -H "Content-Type: application/json" \
  -w "\nHTTP Status: %{http_code}\n\n"

# 2. Create product
echo -e "${YELLOW}2. Create Product${NC}"
curl -X POST "$BASE_URL/api/products" \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Laptop",
    "description": "High-performance laptop for development",
    "initial_stock": 50
  }' \
  -w "\nHTTP Status: %{http_code}\n\n"

# 3. Create another product
echo -e "${YELLOW}3. Create Another Product${NC}"
curl -X POST "$BASE_URL/api/products" \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Mouse",
    "description": "Wireless mouse",
    "initial_stock": 200
  }' \
  -w "\nHTTP Status: %{http_code}\n\n"

# 4. Get product by ID
echo -e "${YELLOW}4. Get Product by ID (ID: 1)${NC}"
curl -X GET "$BASE_URL/api/products/1" \
  -H "Content-Type: application/json" \
  -w "\nHTTP Status: %{http_code}\n\n"

# 5. Search products
echo -e "${YELLOW}5. Search Products (Search: 'Laptop')${NC}"
curl -X GET "$BASE_URL/api/products/search?name=Laptop" \
  -H "Content-Type: application/json" \
  -w "\nHTTP Status: %{http_code}\n\n"

# 6. Update product
echo -e "${YELLOW}6. Update Product (ID: 1)${NC}"
curl -X PUT "$BASE_URL/api/products/1" \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Gaming Laptop",
    "description": "High-performance gaming laptop with RTX 3080"
  }' \
  -w "\nHTTP Status: %{http_code}\n\n"

# 7. Delete product
echo -e "${YELLOW}7. Delete Product (ID: 1)${NC}"
curl -X DELETE "$BASE_URL/api/products/1" \
  -H "Content-Type: application/json" \
  -w "\nHTTP Status: %{http_code}\n\n"

# ==========================================
# INVENTORY API TESTS
# ==========================================
echo -e "${BLUE}=== INVENTORY API ===${NC}\n"

# 1. Get inventory
echo -e "${YELLOW}1. Get Inventory (Product ID: 1)${NC}"
curl -X GET "$BASE_URL/api/inventory/1" \
  -H "Content-Type: application/json" \
  -w "\nHTTP Status: %{http_code}\n\n"

# 2. Update stock
echo -e "${YELLOW}2. Update Stock (Product ID: 1, New Stock: 100)${NC}"
curl -X PUT "$BASE_URL/api/inventory/1" \
  -H "Content-Type: application/json" \
  -d '{
    "stock": 100
  }' \
  -w "\nHTTP Status: %{http_code}\n\n"

# 3. Update stock again
echo -e "${YELLOW}3. Update Stock Again (Product ID: 1, New Stock: 75)${NC}"
curl -X PUT "$BASE_URL/api/inventory/1" \
  -H "Content-Type: application/json" \
  -d '{
    "stock": 75
  }' \
  -w "\nHTTP Status: %{http_code}\n\n"

# ==========================================
# USERS API TESTS
# ==========================================
echo -e "${BLUE}=== USERS API ===${NC}\n"

# 1. Get all users
echo -e "${YELLOW}1. Get All Users${NC}"
curl -X GET "$BASE_URL/api/users" \
  -H "Content-Type: application/json" \
  -w "\nHTTP Status: %{http_code}\n\n"

# 2. Create user (admin)
echo -e "${YELLOW}2. Create Admin User${NC}"
curl -X POST "$BASE_URL/api/users" \
  -H "Content-Type: application/json" \
  -d '{
    "name": "John Admin",
    "email": "john.admin@inventory.com",
    "role": "admin"
  }' \
  -w "\nHTTP Status: %{http_code}\n\n"

# 3. Create user (regular)
echo -e "${YELLOW}3. Create Regular User${NC}"
curl -X POST "$BASE_URL/api/users" \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Jane User",
    "email": "jane.user@inventory.com",
    "role": "user"
  }' \
  -w "\nHTTP Status: %{http_code}\n\n"

# 4. Get user by ID
echo -e "${YELLOW}4. Get User by ID (ID: 1)${NC}"
curl -X GET "$BASE_URL/api/users/1" \
  -H "Content-Type: application/json" \
  -w "\nHTTP Status: %{http_code}\n\n"

# 5. Update user
echo -e "${YELLOW}5. Update User (ID: 1)${NC}"
curl -X PUT "$BASE_URL/api/users/1" \
  -H "Content-Type: application/json" \
  -d '{
    "name": "John Updated",
    "email": "john.updated@inventory.com",
    "role": "admin"
  }' \
  -w "\nHTTP Status: %{http_code}\n\n"

# 6. Delete user
echo -e "${YELLOW}6. Delete User (ID: 2)${NC}"
curl -X DELETE "$BASE_URL/api/users/2" \
  -H "Content-Type: application/json" \
  -w "\nHTTP Status: %{http_code}\n\n"

# ==========================================
# SUBSCRIPTIONS API TESTS
# ==========================================
echo -e "${BLUE}=== SUBSCRIPTIONS API ===${NC}\n"

# 1. Get all subscriptions
echo -e "${YELLOW}1. Get All Subscriptions${NC}"
curl -X GET "$BASE_URL/api/subscriptions" \
  -H "Content-Type: application/json" \
  -w "\nHTTP Status: %{http_code}\n\n"

# 2. Create subscription
echo -e "${YELLOW}2. Create Subscription (User: 1, Product: 1)${NC}"
curl -X POST "$BASE_URL/api/subscriptions" \
  -H "Content-Type: application/json" \
  -d '{
    "user_id": 1,
    "product_id": 1
  }' \
  -w "\nHTTP Status: %{http_code}\n\n"

# 3. Create another subscription
echo -e "${YELLOW}3. Create Another Subscription (User: 1, Product: 2)${NC}"
curl -X POST "$BASE_URL/api/subscriptions" \
  -H "Content-Type: application/json" \
  -d '{
    "user_id": 1,
    "product_id": 2
  }' \
  -w "\nHTTP Status: %{http_code}\n\n"

# 4. Get subscription by ID
echo -e "${YELLOW}4. Get Subscription by ID (ID: 1)${NC}"
curl -X GET "$BASE_URL/api/subscriptions/1" \
  -H "Content-Type: application/json" \
  -w "\nHTTP Status: %{http_code}\n\n"

# 5. Get user subscriptions
echo -e "${YELLOW}5. Get User Subscriptions (User ID: 1)${NC}"
curl -X GET "$BASE_URL/api/users/1/subscriptions" \
  -H "Content-Type: application/json" \
  -w "\nHTTP Status: %{http_code}\n\n"

# 6. Update subscription status (deactivate)
echo -e "${YELLOW}6. Deactivate Subscription (ID: 1)${NC}"
curl -X PUT "$BASE_URL/api/subscriptions/1" \
  -H "Content-Type: application/json" \
  -d '{
    "active": false
  }' \
  -w "\nHTTP Status: %{http_code}\n\n"

# 7. Update subscription status (activate)
echo -e "${YELLOW}7. Activate Subscription (ID: 1)${NC}"
curl -X PUT "$BASE_URL/api/subscriptions/1" \
  -H "Content-Type: application/json" \
  -d '{
    "active": true
  }' \
  -w "\nHTTP Status: %{http_code}\n\n"

# 8. Delete subscription
echo -e "${YELLOW}8. Delete Subscription (ID: 2)${NC}"
curl -X DELETE "$BASE_URL/api/subscriptions/2" \
  -H "Content-Type: application/json" \
  -w "\nHTTP Status: %{http_code}\n\n"

# ==========================================
# ERROR HANDLING TESTS
# ==========================================
echo -e "${BLUE}=== ERROR HANDLING TESTS ===${NC}\n"

# 1. Missing required fields
echo -e "${YELLOW}1. Create Product - Missing Fields${NC}"
curl -X POST "$BASE_URL/api/products" \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Incomplete Product"
  }' \
  -w "\nHTTP Status: %{http_code}\n\n"

# 2. Invalid role
echo -e "${YELLOW}2. Create User - Invalid Role${NC}"
curl -X POST "$BASE_URL/api/users" \
  -H "Content-Type: application/json" \
  -d '{
    "name": "Invalid User",
    "email": "invalid@inventory.com",
    "role": "superuser"
  }' \
  -w "\nHTTP Status: %{http_code}\n\n"

# 3. Invalid stock update
echo -e "${YELLOW}3. Update Stock - Missing Stock Field${NC}"
curl -X PUT "$BASE_URL/api/inventory/1" \
  -H "Content-Type: application/json" \
  -d '{}' \
  -w "\nHTTP Status: %{http_code}\n\n"

# 4. Search without name parameter
echo -e "${YELLOW}4. Search Products - Missing Name Parameter${NC}"
curl -X GET "$BASE_URL/api/products/search" \
  -H "Content-Type: application/json" \
  -w "\nHTTP Status: %{http_code}\n\n"

echo -e "${GREEN}=========================================${NC}"
echo -e "${GREEN}All tests completed!${NC}"
echo -e "${GREEN}=========================================${NC}"
