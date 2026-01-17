# ========================
# Compiler & flags
# ========================
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# ========================
# Include paths
# ========================
INCLUDES = \
-Isrc \
-Isrc/external \
-I/opt/homebrew/include

# ========================
# Libraries
# ========================
LIBS = \
-L/opt/homebrew/lib \
-L/opt/homebrew/opt/libpq/lib \
-lpqxx -lpq

# ========================
# Source files
# ========================
SRCS = \
src/main.cpp \
src/controller/ProductController.cpp \
src/controller/UserController.cpp \
src/controller/SubscriptionController.cpp \
src/service/implementations/InventoryService.cpp \
src/service/implementations/UserService.cpp \
src/service/implementations/SubscriptionService.cpp \
src/repository/postgres/ProductRepo.cpp \
src/repository/postgres/UserRepo.cpp \
src/repository/postgres/SubscriptionRepo.cpp

# ========================
# Output binary
# ========================
TARGET = inventory_api

# ========================
# Build rules
# ========================
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRCS) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild
