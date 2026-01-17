#!/bin/bash
# Quick Start Script - Both Backend and Frontend
# This script starts both services for testing

cd /Users/pritam/inventory-project

echo "🚀 Starting Inventory Management System"
echo "=========================================="

# Start Backend
echo ""
echo "📦 Starting Backend API on http://localhost:8080..."
./inventory_api &
BACKEND_PID=$!
sleep 2

# Start Frontend
echo "⚛️  Starting Frontend on http://localhost:3000..."
cd frontend
npm start &
FRONTEND_PID=$!

echo ""
echo "✅ Both services are starting..."
echo ""
echo "Backend PID: $BACKEND_PID"
echo "Frontend PID: $FRONTEND_PID"
echo ""
echo "🌐 Open browser: http://localhost:3000"
echo ""
echo "To stop services:"
echo "  kill $BACKEND_PID $FRONTEND_PID"
echo ""

# Keep script running
wait
