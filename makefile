# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -pthread

# Add static linking flags
LDFLAGS = -static

# Directories
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

# Source files and object files for the main chess engine
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Object files for the test program
TEST_SRC = test_chess.cpp
TEST_OBJ = $(OBJ_DIR)/test_chess.o

# Output executables
EXECUTABLE = chess_engine
TEST_EXECUTABLE = test_chess
ENGINE_EXECUTABLE = hakaisha # New target for the static chess engine

# Default target
all: $(EXECUTABLE)

# Create chess engine executable
$(EXECUTABLE): $(OBJ_FILES) $(OBJ_DIR)/main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile main.o separately
$(OBJ_DIR)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

# Compile all .cpp files in src/ to .o files in obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test_chess.o
$(TEST_OBJ): $(TEST_SRC) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(TEST_SRC) -o $(TEST_OBJ)

# Create test_chess executable for testing
test: $(OBJ_FILES) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TEST_EXECUTABLE) $^

# Create static hakaisha engine executable
engine: $(OBJ_FILES) $(OBJ_DIR)/main.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(ENGINE_EXECUTABLE) $^

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up the build files
clean:
	rm -rf $(OBJ_DIR)/*.o $(EXECUTABLE) $(ENGINE_EXECUTABLE) $(TEST_EXECUTABLE)

.PHONY: all test clean engine
