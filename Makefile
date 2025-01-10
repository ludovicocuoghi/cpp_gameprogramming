# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -I/opt/homebrew/opt/sfml@2/include -Iimgui -Iimgui-sfml
LDFLAGS = -L/opt/homebrew/opt/sfml@2/lib -lsfml-graphics -lsfml-window -lsfml-system -framework OpenGL

# Project files
SRC = main.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui-sfml/imgui-SFML.cpp
BIN = bin/sfml_app

# Default target
all: $(BIN)

$(BIN): $(SRC)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)

clean:
	rm -rf bin
