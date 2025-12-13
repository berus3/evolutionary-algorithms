CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -fPIC \
           -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux

SRC_DIR = src
BUILD_DIR = build

# todo menos los que tienen main()
COMMON_SRCS = $(filter-out $(SRC_DIR)/main.cpp $(SRC_DIR)/test_build.cpp, $(wildcard $(SRC_DIR)/*.cpp))
COMMON_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(COMMON_SRCS))

APP_TARGET  = combat_app
TEST_TARGET = combat_test
SHARED_LIB  = libcombat.so

all: $(APP_TARGET) $(TEST_TARGET) $(SHARED_LIB)

$(APP_TARGET): $(COMMON_OBJS) $(BUILD_DIR)/main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_TARGET): $(COMMON_OBJS) $(BUILD_DIR)/test_build.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# la .so NO debe incluir main()
$(SHARED_LIB): $(COMMON_OBJS)
	$(CXX) -shared -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(APP_TARGET) $(TEST_TARGET) $(SHARED_LIB)

.PHONY: all clean
