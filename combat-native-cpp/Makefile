CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -fPIC \
           -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux

SRC_DIR = src
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

TARGET = combat_app
SHARED_LIB = libcombat.so

all: $(TARGET) $(SHARED_LIB)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SHARED_LIB): $(OBJS)
	$(CXX) -shared -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(SHARED_LIB)

.PHONY: all clean
