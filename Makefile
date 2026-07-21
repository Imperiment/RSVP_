CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -Ithird_party/CLI11/include
LDFLAGS :=

PKG_LIBS := poppler-cpp ncurses
PKG_CFLAGS := $(shell pkg-config --cflags $(PKG_LIBS))
PKG_LDLIBS := $(shell pkg-config --libs $(PKG_LIBS))

CXXFLAGS += $(PKG_CFLAGS)
LDLIBS := $(PKG_LDLIBS)

SRC_DIR := src
BUILD_DIR := build
TEST_DIR := tests
TARGET := pacer

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

LOGIC_SRCS := $(filter-out $(SRC_DIR)/main.cpp,$(SRCS))
LOGIC_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(LOGIC_SRCS))

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	./$(TARGET) $(ARGS)

# --- Testing ---
TEST_CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -Ithird_party/doctest

tokenizer_tests: $(TEST_DIR)/tokenizer_tests.cpp $(SRC_DIR)/tokenizer.cpp
	$(CXX) $(TEST_CXXFLAGS) $^ -o $(BUILD_DIR)/tokenizer_tests
	./$(BUILD_DIR)/tokenizer_tests

pacing_tests: $(TEST_DIR)/pacing_tests.cpp $(SRC_DIR)/pacing.cpp
	$(CXX) $(TEST_CXXFLAGS) $^ -o $(BUILD_DIR)/pacing_tests
	./$(BUILD_DIR)/pacing_tests

test: | $(BUILD_DIR)
	$(MAKE) tokenizer_tests
	$(MAKE) pacing_tests

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
