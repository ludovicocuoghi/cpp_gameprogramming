
# SFML Shape Collision & GUI Control

This project is a **real-time collision simulation** using **SFML (Simple and Fast Multimedia Library)** with a graphical user interface powered by **ImGui** and **ImGui-SFML**. It allows users to manage shapes, control their properties dynamically (such as size, speed, and color), and visualize advanced collision behaviors.

---

## Features

### 1. **Collision Detection**
- Implements real-time collision detection between shapes.
- Shapes "bounce" upon collision with boundaries or other shapes.
- Speed increases dynamically on collision (multiplied by -1.3) to simulate energetic bounces.
- When collisions end, shapes revert to their original speed but with an inverted direction to maintain realistic motion.

### 2. **ImGui Integration**
- Shapes can be customized dynamically via a GUI.
- The GUI uses ImGui-SFML to allow:
  - **Shape Selection:** A dropdown menu to select shapes.
  - **Size Adjustment:** Adjust the width/height (for rectangles) or radius (for circles).
  - **Color Customization:** RGB sliders to choose a custom color for the shape.
  - **Speed Control:** Modify the x and y velocities in real-time.

### 3. **Color Normalization**
- Colors are normalized to the [0.0, 1.0] range for the GUI and denormalized back to [0, 255] for SFML.
- This ensures accurate color representation and smooth transitions when modifying colors in the GUI.

### 4. **Boundary Collision**
- Shapes detect and respond to window boundaries, reversing their direction when hitting edges.
- Shape positions are clamped to avoid sticking to the edges.

---

## Installation & Setup

### Prerequisites
- **C++ Compiler** supporting C++20 or later.
- **SFML 2.6.x** installed via Homebrew or any other method.
- **ImGui** and **ImGui-SFML** for GUI integration.

### Installing Dependencies
1. Install SFML 2.6.x (if not already installed):
   ```bash
   brew install sfml@2
   ```
2. Clone the required libraries:
   ```bash
   git clone https://github.com/ocornut/imgui.git
   cd imgui
   git checkout v1.89.8
   cd ..

   git clone https://github.com/eliasdaler/imgui-sfml.git
   cd imgui-sfml
   git checkout v2.6
   cd ..
   ```

### Building the Project
1. Update the `Makefile` with the correct include and library paths:
   ```makefile
   CXX = g++
   CXXFLAGS = -std=c++20 -I/opt/homebrew/opt/sfml@2/include -Iimgui -Iimgui-sfml
   LDFLAGS = -L/opt/homebrew/opt/sfml@2/lib -lsfml-graphics -lsfml-window -lsfml-system -framework OpenGL

   SRC = main.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui-sfml/imgui-SFML.cpp
   BIN = bin/sfml_app

   all: $(BIN)

   $(BIN): $(SRC)
       @mkdir -p bin
       $(CXX) $(CXXFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)

   clean:
       rm -rf bin
   ```
2. Build and run the project:
   ```bash
   make clean
   make
   ./bin/sfml_app
   ```

---

## Usage

### GUI Controls
- **Shape Selection:** Select a shape from the dropdown to modify its properties.
- **Color Editing:** Use the RGB sliders to adjust the shape's color in real-time.
- **Size Adjustment:** Change the width and height for rectangles or radius for circles.
- **Speed Adjustment:** Modify the x and y velocities of shapes.

### Collision System
- Shapes detect collisions with both boundaries and other shapes.
- Upon collision:
  - The speed is dynamically adjusted (multiplied by -1.3).
  - Shapes change color to red during the collision.
  - Once the collision ends, shapes revert to their original speed with the opposite direction.

---

## Code Overview

### Core Files
- **`main.cpp`:** Contains the core logic for shape movement, collision detection, and GUI integration.
- **`Makefile`:** Used to compile the project with all dependencies.

### Key Components
1. **Collision Logic:**
   - Handles shape-boundary and shape-shape collisions.
   - Ensures shapes bounce with realistic speed changes.
   - Collision states are tracked to prevent speed accumulation.

2. **GUI Integration:**
   - Uses ImGui to create a dynamic interface for shape customization.
   - Dropdown menus and sliders simplify interaction.

3. **Color Normalization:**
   - Converts SFML colors to normalized floats for ImGui.
   - Denormalizes the floats back to integers for SFML rendering.

---

## Known Issues
- Ensure the SFML and ImGui-SFML versions are compatible (currently using SFML 2.6.x and ImGui-SFML 2.6).
- Avoid using extremely high velocities to prevent shapes from skipping collision detection.

---

## Future Enhancements
- Add support for new shape types (e.g., triangles).
- Include global controls to pause/resume all shapes or reset the simulation.
- Improve GUI layout to accommodate large numbers of shapes.

---

## Credits
- **SFML:** [Simple and Fast Multimedia Library](https://www.sfml-dev.org/)
- **ImGui:** [Dear ImGui](https://github.com/ocornut/imgui)
- **ImGui-SFML:** [ImGui-SFML Integration](https://github.com/eliasdaler/imgui-sfml)
