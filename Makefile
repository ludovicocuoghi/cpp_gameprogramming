# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system

# Project files
SRC = main.cpp
BIN = bin/sfml_app

# Default target
all: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)

# Clean up build artifacts
clean:
	rm -f $(BIN)
