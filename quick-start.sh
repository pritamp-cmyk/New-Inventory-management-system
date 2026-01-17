#!/bin/bash

# Inventory Management System - Quick Start Script
# This script sets up and starts both backend and frontend

set -e

echo "🚀 Inventory Management System - Quick Start"
echo "==========================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if running from correct directory
if [ ! -f "src/main.cpp" ]; then
    echo -e "${RED}Error: Please run this script from the project root directory${NC}"
    exit 1
fi

# Phase 1: Check Backend
echo -e "\n${YELLOW}Phase 1: Checking Backend...${NC}"
if [ ! -f "src/main" ]; then
    echo -e "${YELLOW}Building backend...${NC}"
    cd src
    make
    cd ..
    echo -e "${GREEN}✓ Backend built${NC}"
else
    echo -e "${GREEN}✓ Backend executable found${NC}"
fi

# Phase 2: Check Database
echo -e "\n${YELLOW}Phase 2: Checking Database...${NC}"
if ! docker ps | grep -q inventory-db; then
    echo -e "${YELLOW}Starting PostgreSQL container...${NC}"
    docker-compose up -d
    sleep 3
    echo -e "${GREEN}✓ Database container started${NC}"
else
    echo -e "${GREEN}✓ Database already running${NC}"
fi

# Phase 3: Check Frontend
echo -e "\n${YELLOW}Phase 3: Checking Frontend...${NC}"
if [ ! -d "frontend/node_modules" ]; then
    echo -e "${YELLOW}Installing frontend dependencies...${NC}"
    cd frontend
    npm install
    cd ..
    echo -e "${GREEN}✓ Frontend dependencies installed${NC}"
else
    echo -e "${GREEN}✓ Frontend dependencies already installed${NC}"
fi

if [ ! -f "frontend/.env" ]; then
    echo -e "${YELLOW}Creating .env file...${NC}"
    cp frontend/.env.example frontend/.env
    echo -e "${GREEN}✓ .env file created${NC}"
fi

# Phase 4: Summary
echo -e "\n${GREEN}=========================================${NC}"
echo -e "${GREEN}✓ All systems ready!${NC}"
echo -e "${GREEN}=========================================${NC}"

echo -e "\n${YELLOW}To start the system:${NC}"
echo ""
echo -e "${YELLOW}Terminal 1 - Backend:${NC}"
echo "  cd /Users/pritam/inventory-project"
echo "  ./src/main"
echo ""
echo -e "${YELLOW}Terminal 2 - Frontend:${NC}"
echo "  cd /Users/pritam/inventory-project/frontend"
echo "  npm start"
echo ""
echo -e "${YELLOW}Then open:${NC}"
echo "  http://localhost:3000"
echo ""
echo -e "${GREEN}Documentation:${NC}"
echo "  - Frontend Guide: FRONTEND_GUIDE.md"
echo "  - Integration Guide: INTEGRATION_GUIDE.md"
echo "  - API Testing: API_DOCUMENTATION.md"
echo ""
