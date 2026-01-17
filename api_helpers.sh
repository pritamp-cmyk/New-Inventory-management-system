#!/bin/bash

# Inventory API Testing Helper Functions
# Add to ~/.zshrc or ~/.bash_profile to use globally

BASE_URL="http://localhost:8080"

# ==========================================
# Helper Functions
# ==========================================

# Get all products
get_products() {
    curl -s "$BASE_URL/api/products" | jq
}

# Get product by ID
get_product() {
    local id=${1:-1}
    curl -s "$BASE_URL/api/products/$id" | jq
}

# Search products
search_products() {
    local name=${1:-""}
    if [ -z "$name" ]; then
        echo "Usage: search_products <product_name>"
        return 1
    fi
    curl -s "$BASE_URL/api/products/search?name=$name" | jq
}

# Create product
create_product() {
    local name=${1:-"Product"}
    local desc=${2:-"Description"}
    local stock=${3:-50}
    curl -s -X POST "$BASE_URL/api/products" \
        -H "Content-Type: application/json" \
        -d "{\"name\":\"$name\",\"description\":\"$desc\",\"initial_stock\":$stock}" | jq
}

# Update product
update_product() {
    local id=${1:-1}
    local name=${2:-"Updated"}
    local desc=${3:-"Updated Desc"}
    curl -s -X PUT "$BASE_URL/api/products/$id" \
        -H "Content-Type: application/json" \
        -d "{\"name\":\"$name\",\"description\":\"$desc\"}" | jq
}

# Delete product
delete_product() {
    local id=${1:-1}
    curl -s -X DELETE "$BASE_URL/api/products/$id" | jq
}

# ==========================================
# Inventory Functions
# ==========================================

# Get inventory
get_inventory() {
    local id=${1:-1}
    curl -s "$BASE_URL/api/inventory/$id" | jq
}

# Update stock
update_stock() {
    local id=${1:-1}
    local stock=${2:-100}
    curl -s -X PUT "$BASE_URL/api/inventory/$id" \
        -H "Content-Type: application/json" \
        -d "{\"stock\":$stock}" | jq
}

# ==========================================
# User Functions
# ==========================================

# Get all users
get_users() {
    curl -s "$BASE_URL/api/users" | jq
}

# Get user by ID
get_user() {
    local id=${1:-1}
    curl -s "$BASE_URL/api/users/$id" | jq
}

# Create user
create_user() {
    local name=${1:-"User"}
    local email=${2:-"user@test.com"}
    local role=${3:-"user"}
    curl -s -X POST "$BASE_URL/api/users" \
        -H "Content-Type: application/json" \
        -d "{\"name\":\"$name\",\"email\":\"$email\",\"role\":\"$role\"}" | jq
}

# Update user
update_user() {
    local id=${1:-1}
    local name=${2:-"Updated"}
    local email=${3:-"updated@test.com"}
    local role=${4:-"user"}
    curl -s -X PUT "$BASE_URL/api/users/$id" \
        -H "Content-Type: application/json" \
        -d "{\"name\":\"$name\",\"email\":\"$email\",\"role\":\"$role\"}" | jq
}

# Delete user
delete_user() {
    local id=${1:-1}
    curl -s -X DELETE "$BASE_URL/api/users/$id" | jq
}

# ==========================================
# Subscription Functions
# ==========================================

# Get all subscriptions
get_subscriptions() {
    curl -s "$BASE_URL/api/subscriptions" | jq
}

# Get subscription by ID
get_subscription() {
    local id=${1:-1}
    curl -s "$BASE_URL/api/subscriptions/$id" | jq
}

# Get user subscriptions
get_user_subscriptions() {
    local user_id=${1:-1}
    curl -s "$BASE_URL/api/users/$user_id/subscriptions" | jq
}

# Create subscription
create_subscription() {
    local user_id=${1:-1}
    local product_id=${2:-1}
    curl -s -X POST "$BASE_URL/api/subscriptions" \
        -H "Content-Type: application/json" \
        -d "{\"user_id\":$user_id,\"product_id\":$product_id}" | jq
}

# Update subscription status
update_subscription() {
    local id=${1:-1}
    local active=${2:-true}
    curl -s -X PUT "$BASE_URL/api/subscriptions/$id" \
        -H "Content-Type: application/json" \
        -d "{\"active\":$active}" | jq
}

# Delete subscription
delete_subscription() {
    local id=${1:-1}
    curl -s -X DELETE "$BASE_URL/api/subscriptions/$id" | jq
}

# ==========================================
# Utility Functions
# ==========================================

# Test if server is running
test_server() {
    if curl -s "$BASE_URL/api/products" > /dev/null 2>&1; then
        echo "✅ Server is running on $BASE_URL"
    else
        echo "❌ Server is not responding at $BASE_URL"
        echo "Start it with: ./inventory_api"
    fi
}

# Run all tests
run_all_tests() {
    echo "Running full test suite..."
    ./test_apis.sh
}

# Show help
api_help() {
    cat << 'EOF'
╔══════════════════════════════════════════════════════════════════╗
║        Inventory API Testing Helper Functions                   ║
╚══════════════════════════════════════════════════════════════════╝

PRODUCTS:
  get_products                      # Get all products
  get_product [id]                  # Get product by ID
  search_products <name>            # Search products
  create_product [name] [desc] [stock]  # Create product
  update_product [id] [name] [desc]     # Update product
  delete_product [id]               # Delete product

INVENTORY:
  get_inventory [id]                # Get stock level
  update_stock [id] [stock]         # Update stock

USERS:
  get_users                         # Get all users
  get_user [id]                     # Get user by ID
  create_user [name] [email] [role] # Create user
  update_user [id] [name] [email] [role] # Update user
  delete_user [id]                  # Delete user

SUBSCRIPTIONS:
  get_subscriptions                 # Get all subscriptions
  get_subscription [id]             # Get subscription by ID
  get_user_subscriptions [user_id]  # Get user subscriptions
  create_subscription [user_id] [product_id] # Subscribe
  update_subscription [id] [active] # Update subscription status
  delete_subscription [id]          # Unsubscribe

UTILITIES:
  test_server                       # Test if server is running
  run_all_tests                     # Run full test suite
  api_help                          # Show this help

EXAMPLES:
  get_products
  create_product "Laptop" "Gaming Laptop" 50
  update_stock 1 100
  create_user "John" "john@test.com" "admin"
  create_subscription 1 1
  search_products "Laptop"

EOF
}

# ==========================================
# Quick Stats
# ==========================================

# Get API stats
api_stats() {
    echo "📊 API Statistics"
    echo "===================="
    
    echo "🛍️  Products:"
    curl -s "$BASE_URL/api/products" | jq 'length' && echo "  items"
    
    echo ""
    echo "👥 Users:"
    curl -s "$BASE_URL/api/users" | jq 'length' && echo "  items"
    
    echo ""
    echo "📋 Subscriptions:"
    curl -s "$BASE_URL/api/subscriptions" | jq 'length' && echo "  items"
}

# Export functions (uncomment if adding to shell profile)
# export -f get_products
# export -f get_product
# export -f create_product
# ... etc

# Show help if script is run directly
if [ "$0" = "${BASH_SOURCE[0]}" ]; then
    api_help
fi
