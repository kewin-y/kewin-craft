CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I./include -ggdb3
LDFLAGS := -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
EXEC := kwnc

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

.PHONY: all
all: $(BIN_DIR)/$(EXEC)

$(BIN_DIR)/$(EXEC): $(OBJ) $(OBJ_DIR)/gl.o
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/gl.o: $(SRC_DIR)/gl.c
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: test
test: $(BIN_DIR)/$(EXEC)
	./$<

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
