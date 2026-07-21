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
TARGET := pacer

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	./$(TARGET) $(ARGS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
