
# SFML Collision Detection Simulation

## Overview
This project demonstrates a dynamic simulation of moving shapes with realistic collision detection and response, built using the **SFML (Simple and Fast Multimedia Library)**. The program allows shapes (rectangles and circles) to move across the screen, interact with each other, and bounce off window boundaries, showcasing advanced collision handling.


## Demo Video
[VIDEO DEMO](https://www.youtube.com/watch?v=rqzfr4WHKeI)

---

## Features
- **Dynamic Movement:** Shapes move with configurable speeds and directions.
- **Collision Detection:**
  - Shapes reverse their speed and direction upon collision.
  - Speed increases dynamically during collisions to create realistic bounce effects.
- **Boundary Reflection:**
  - Shapes bounce off the window boundaries while maintaining correct positions.
- **Text Alignment:** Shape names dynamically follow their respective shapes during movement.
- **Customizable Configuration:**
  - The `config.txt` file allows users to define shape properties such as size, color, position, and speed.

---

## Collision Algorithm Explained
This project uses an **advanced collision detection algorithm** to handle shape interactions realistically. Here’s how it works:

### Challenges with Basic Collision
In simple collision detection:
1. When two shapes collide, their speeds and directions are reversed.
2. However, because shapes typically remain intersecting in the next frame, they repeatedly "collide" in subsequent frames.
3. This leads to the shapes getting stuck in a loop of moving closer and farther, resulting in unnatural behavior.

### Solution: Storing Speed States
To solve this, the algorithm:
1. **Tracks Collision States:**  
   Each shape has a `CollisionState` that stores:
   - Whether the shape is currently in a collision (`inCollision`).
   - The original speed of the shape before the collision (`originalXSpeed` and `originalYSpeed`).

2. **Adjusts Speed Dynamically:**  
   - On collision:
     - The shape’s speed is reversed and slightly increased (e.g., `*=-1.3`) to create a realistic bounce effect.
   - After separation:
     - The shape’s speed is reset to the original values (but reversed in direction) to prevent sticking.

3. **Boundary Reflection Handling:**  
   - The position is adjusted to ensure shapes never go out of bounds.
   - Collision states are managed independently for boundary collisions.

### Why This Algorithm is Advanced
- **Avoids Sticking:**  
  Shapes do not get stuck together because the algorithm resets their speeds after separation.
- **Dynamic Interaction:**  
  The speed increase during collisions creates realistic bouncing behavior.
- **Independent Collision Management:**  
  Boundary collisions are handled separately, ensuring smooth interactions.

---

## How to Use
1. **Install SFML:**  
   Download and set up SFML from [SFML's official website](https://www.sfml-dev.org/).

2. **Prepare Configuration File:**  
   Edit `config.txt` to define your shapes:
   ```txt
   window 800 600
   rectangle Rect1 100 100 2 3 255 0 0 50 50
   circle Circle1 300 200 -3 1 0 255 0 30
   ```

3. **Run the Program:**  
   Compile and execute the program to see the shapes in action.

---

## Configuration File Details
The `config.txt` file defines the simulation settings:
- **Window Dimensions:**  
  ```
  window <width> <height>
  ```
- **Shape Definitions:**  
  ```
  rectangle <name> <x> <y> <xSpeed> <ySpeed> <R> <G> <B> <width> <height>
  circle <name> <x> <y> <xSpeed> <ySpeed> <R> <G> <B> <radius>
  ```

---

## Demo Video
[![Watch the video](https://img.youtube.com/vi/YourVideoID/0.jpg)](https://www.youtube.com/watch?v=YourVideoID)

Click the image above to watch a demo of the project in action.

---

## Code Example
Here’s an example of the collision handling logic:

```cpp
if (bounds1.intersects(bounds2)) {
    isColliding = true;

    if (!state.inCollision) {
        state.originalXSpeed = config.xSpeed;
        state.originalYSpeed = config.ySpeed;

        config.xSpeed *= -1.3f;
        config.ySpeed *= -1.3f;

        shape->setFillColor(sf::Color::Red);
    }
    state.inCollision = true;
}

// Reset speed when no longer colliding
if (!isColliding && state.inCollision) {
    config.xSpeed = -state.originalXSpeed;
    config.ySpeed = -state.originalYSpeed;
    state.inCollision = false;
}
```

---

## Technical Details
- **Language:** C++
- **Library:** SFML (Simple and Fast Multimedia Library)
- **Customizable Input:** Via `config.txt`

---

## Acknowledgements
This project showcases the power and flexibility of SFML for multimedia applications, along with advanced collision detection logic for interactive simulations.
