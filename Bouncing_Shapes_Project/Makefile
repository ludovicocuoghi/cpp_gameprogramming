# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -I/opt/homebrew/opt/sfml@2/include -Iimgui -Iimgui-sfml
LDFLAGS = -L/opt/homebrew/opt/sfml@2/lib -lsfml-graphics -lsfml-window -lsfml-system -framework OpenGL

# Project files
SRC_MAIN = main.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui-sfml/imgui-SFML.cpp
SRC_EZC = easy_imgui.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui-sfml/imgui-SFML.cpp
# Binaries
BIN_MAIN = bin/sfml_app
BIN_EZC = bin/easy_imgui_app
# Default target
all: $(BIN_MAIN) $(BIN_EZC) $(BIN_DEMO)

# Compile main.cpp target
$(BIN_MAIN): $(SRC_MAIN)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SRC_MAIN) -o $(BIN_MAIN) $(LDFLAGS)

# Compile EZC.cpp target
$(BIN_EZC): $(SRC_EZC)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SRC_EZC) -o $(BIN_EZC) $(LDFLAGS)

# Clean target
clean:
	rm -rf bin
