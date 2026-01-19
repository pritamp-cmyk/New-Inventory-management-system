#!/bin/bash

# Color codes for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

API_URL="http://localhost:8080/api"

echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}   NOTIFICATION SYSTEM TEST SUITE${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}\n"

# Test 1: Check if backend is running
echo -e "${YELLOW}[TEST 1] Checking if backend is running...${NC}"
if curl -s http://localhost:8080/api/products > /dev/null 2>&1; then
    echo -e "${GREEN}✅ Backend is running${NC}\n"
else
    echo -e "${RED}❌ Backend is not running. Start with: ./inventory_api${NC}\n"
    exit 1
fi

# Test 2: Get existing users and products
echo -e "${YELLOW}[TEST 2] Fetching existing users and products...${NC}"
USERS=$(curl -s $API_URL/users | jq '.[] | {id, name, email}')
PRODUCTS=$(curl -s $API_URL/products | jq '.[] | {id, name, stock}')

echo "📋 Existing Users:"
echo "$USERS" | jq . 2>/dev/null || echo "No users found"
echo ""

echo "📦 Existing Products:"
echo "$PRODUCTS" | jq . 2>/dev/null || echo "No products found"
echo ""

# Extract first user and product IDs
USER_ID=$(curl -s $API_URL/users | jq -r '.[0].id' 2>/dev/null)
PRODUCT_ID=$(curl -s $API_URL/products | jq -r '.[0].id' 2>/dev/null)

# If no users exist, create one
if [[ "$USER_ID" == "null" ]] || [[ -z "$USER_ID" ]]; then
    echo -e "${YELLOW}[TEST 3] Creating test user...${NC}"
    USER_RESPONSE=$(curl -s -X POST $API_URL/users \
      -H "Content-Type: application/json" \
      -d '{
        "name": "Test User",
        "email": "test@example.com",
        "role": "customer"
      }')
    USER_ID=$(echo $USER_RESPONSE | jq -r '.id' 2>/dev/null)
    echo -e "${GREEN}✅ User created with ID: $USER_ID${NC}\n"
else
    echo -e "${GREEN}✅ Using existing user ID: $USER_ID${NC}\n"
fi

# If no products exist, create one
if [[ "$PRODUCT_ID" == "null" ]] || [[ -z "$PRODUCT_ID" ]]; then
    echo -e "${YELLOW}[TEST 4] Creating test product...${NC}"
    PRODUCT_RESPONSE=$(curl -s -X POST $API_URL/products \
      -H "Content-Type: application/json" \
      -d '{
        "name": "Test Product",
        "description": "A test product for notification testing",
        "price": 29.99
      }')
    PRODUCT_ID=$(echo $PRODUCT_RESPONSE | jq -r '.id' 2>/dev/null)
    echo -e "${GREEN}✅ Product created with ID: $PRODUCT_ID${NC}\n"
else
    echo -e "${GREEN}✅ Using existing product ID: $PRODUCT_ID${NC}\n"
fi

# Test 5: Subscribe user to product
echo -e "${YELLOW}[TEST 5] Subscribing user $USER_ID to product $PRODUCT_ID...${NC}"
SUBSCRIBE_RESPONSE=$(curl -s -X POST $API_URL/notifications/subscribe \
  -H "Content-Type: application/json" \
  -d "{\"user_id\": $USER_ID, \"product_id\": $PRODUCT_ID}")

SUBSCRIBE_STATUS=$(echo $SUBSCRIBE_RESPONSE | jq -r '.status' 2>/dev/null)
if [[ "$SUBSCRIBE_STATUS" == "success" ]]; then
    echo -e "${GREEN}✅ Successfully subscribed to notifications${NC}"
    echo "Response: $(echo $SUBSCRIBE_RESPONSE | jq .)"
else
    echo -e "${RED}❌ Subscription failed${NC}"
    echo "Response: $(echo $SUBSCRIBE_RESPONSE | jq .)"
fi
echo ""

# Test 6: Set product stock to 0 (out of stock)
echo -e "${YELLOW}[TEST 6] Setting product stock to 0 (out of stock)...${NC}"
OUT_OF_STOCK=$(curl -s -X PUT $API_URL/inventory/$PRODUCT_ID \
  -H "Content-Type: application/json" \
  -d '{"stock": 0}')
echo "Response: $(echo $OUT_OF_STOCK | jq .)"
echo ""

# Test 7: Restock product (stock > 0) - THIS SHOULD TRIGGER NOTIFICATIONS
echo -e "${YELLOW}[TEST 7] Restocking product (setting stock to 50) - SHOULD TRIGGER NOTIFICATIONS...${NC}"
RESTOCK_RESPONSE=$(curl -s -X PUT $API_URL/inventory/$PRODUCT_ID \
  -H "Content-Type: application/json" \
  -d '{"stock": 50}')

TRIGGERED=$(echo $RESTOCK_RESPONSE | jq -r '.notifications_triggered' 2>/dev/null)
echo "Response: $(echo $RESTOCK_RESPONSE | jq .)"

if [[ "$TRIGGERED" == "true" ]]; then
    echo -e "${GREEN}✅ Notifications were triggered on restock!${NC}\n"
else
    echo -e "${YELLOW}⚠️  Notifications not triggered (check if product was out of stock)${NC}\n"
fi

# Test 8: Check notification logs
echo -e "${YELLOW}[TEST 8] Checking notification logs for user $USER_ID...${NC}"
LOGS=$(curl -s $API_URL/notifications/logs/user/$USER_ID | jq '.')
echo "Logs:"
echo $LOGS | jq '.[] | {id, type, status, message, retry_count, created_at}' 2>/dev/null || echo "No logs found"
echo ""

# Test 9: Check user subscriptions
echo -e "${YELLOW}[TEST 9] Checking active subscriptions for user $USER_ID...${NC}"
SUBSCRIPTIONS=$(curl -s $API_URL/notifications/user/$USER_ID | jq '.')
echo "Subscriptions:"
echo $SUBSCRIPTIONS | jq '.[] | {id, product_id, type, is_sent, created_at}' 2>/dev/null || echo "No subscriptions found"
echo ""

# Test 10: Check for failed notifications
echo -e "${YELLOW}[TEST 10] Checking for failed notifications...${NC}"
FAILED=$(curl -s $API_URL/notifications/logs/status/failed | jq '.')
FAILED_COUNT=$(echo $FAILED | jq 'length' 2>/dev/null)
if [[ "$FAILED_COUNT" -gt 0 ]]; then
    echo -e "${RED}⚠️  Found $FAILED_COUNT failed notifications:${NC}"
    echo $FAILED | jq '.[] | {id, error_message, retry_count, max_retries}' 2>/dev/null
else
    echo -e "${GREEN}✅ No failed notifications${NC}"
fi
echo ""

# Test 11: Get notification preferences
echo -e "${YELLOW}[TEST 11] Checking notification preferences for user $USER_ID...${NC}"
PREFS=$(curl -s $API_URL/notifications/preferences/$USER_ID | jq '.')
echo "Preferences:"
echo $PREFS | jq '{email_enabled, push_enabled, sms_enabled, in_app_enabled}' 2>/dev/null || echo "No preferences found"
echo ""

# Test 12: Unsubscribe
echo -e "${YELLOW}[TEST 12] Unsubscribing from product...${NC}"
# Get the subscription ID from user's subscriptions
SUBSCRIPTION_ID=$(curl -s $API_URL/notifications/user/$USER_ID | jq -r '.[0].id' 2>/dev/null)
if [[ "$SUBSCRIPTION_ID" != "null" ]] && [[ ! -z "$SUBSCRIPTION_ID" ]]; then
    UNSUB_RESPONSE=$(curl -s -X DELETE $API_URL/notifications/$SUBSCRIPTION_ID)
    UNSUB_STATUS=$(echo $UNSUB_RESPONSE | jq -r '.status' 2>/dev/null)
    if [[ "$UNSUB_STATUS" == "success" ]]; then
        echo -e "${GREEN}✅ Successfully unsubscribed${NC}"
    else
        echo -e "${RED}❌ Unsubscription failed${NC}"
    fi
    echo "Response: $(echo $UNSUB_RESPONSE | jq .)"
else
    echo -e "${YELLOW}⚠️  No subscription to unsubscribe from${NC}"
fi
echo ""

# Test Summary
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}   TEST SUMMARY${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}\n"

echo -e "${GREEN}✅ Test Configuration:${NC}"
echo "   User ID: $USER_ID"
echo "   Product ID: $PRODUCT_ID"
echo ""

if [[ "$TRIGGERED" == "true" ]]; then
    echo -e "${GREEN}✅ NOTIFICATION SYSTEM IS WORKING!${NC}"
    echo "   - Subscription successful"
    echo "   - Restock triggered notifications"
    echo "   - Check logs for notification details"
else
    echo -e "${YELLOW}⚠️  VERIFY NOTIFICATION DETAILS${NC}"
    echo "   - Check if product was set to 0 before restocking"
    echo "   - Check backend console for error messages"
    echo "   - Review notification logs above"
fi
echo ""
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}\n"
