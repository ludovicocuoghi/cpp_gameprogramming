#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory> // For std::unique_ptr

// Structure to store configuration for each shape
struct ShapeConfig {
    std::string type;    // "rectangle" or "circle"
    std::string name;    // Name of the shape
    sf::Color color;     // Color of the shape (RGB)
    float x, y;          // Initial position
    float xSpeed, ySpeed;// Speed in x and y directions
    float width, height; // Width and height for rectangles
    float radius;        // Radius for circles
};

// Structure to track collision states for each shape
struct CollisionState {
    bool inCollision;        // True if the shape is in a collision
    float originalXSpeed;    // Original horizontal speed before collision
    float originalYSpeed;    // Original vertical speed before collision
};

int main() {
    // Open configuration file
    std::ifstream configFile("config.txt");
    if (!configFile.is_open()) {
        std::cerr << "Could not open config.txt.\n";
        return -1;
    }

    // Parse window size and shape configurations
    int windowWidth = 800, windowHeight = 600;
    std::vector<ShapeConfig> shapes;
    std::string line;

    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "window") {
            // Set window size
            iss >> windowWidth >> windowHeight;
        } else if (type == "rectangle" || type == "circle") {
            ShapeConfig shape;
            shape.type = type;

            // Read name, position, speed, and color
            iss >> shape.name >> shape.x >> shape.y >> shape.xSpeed >> shape.ySpeed;
            int r, g, b;
            iss >> r >> g >> b;
            shape.color = sf::Color(r, g, b);

            // Read size based on shape type
            if (type == "rectangle") {
                iss >> shape.width >> shape.height;
            } else if (type == "circle") {
                iss >> shape.radius;
            }
            shapes.push_back(shape);
        }
    }

    // Load font for text labels
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Could not load font file.\n";
        return -1;
    }

    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Collision Detection");
    window.setFramerateLimit(60);

    // Create SFML shapes and their text labels
    std::vector<std::unique_ptr<sf::Shape>> sfmlShapes;
    std::vector<sf::Text> sfmlTexts;

    for (const auto& shape : shapes) {
        // Create text for the shape's label
        sf::Text text;
        text.setFont(font);
        text.setString(shape.name);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(24);

        // Create the shape based on its type
        if (shape.type == "rectangle") {
            auto rect = std::make_unique<sf::RectangleShape>(sf::Vector2f(shape.width, shape.height));
            rect->setPosition(shape.x, shape.y);
            rect->setFillColor(shape.color);
            sfmlShapes.push_back(std::move(rect));
        } else if (shape.type == "circle") {
            auto circ = std::make_unique<sf::CircleShape>(shape.radius);
            circ->setPosition(shape.x, shape.y);
            circ->setFillColor(shape.color);
            sfmlShapes.push_back(std::move(circ));
        }

        // Center the text on the shape
        auto bounds = sfmlShapes.back()->getGlobalBounds();
        auto textBounds = text.getLocalBounds();
        text.setPosition(bounds.left + bounds.width / 2.f - textBounds.width / 2.f,
                         bounds.top + bounds.height / 2.f - textBounds.height / 2.f - textBounds.top);
        sfmlTexts.push_back(text);
    }

    // Initialize collision states for all shapes
    std::vector<CollisionState> collisionStates(shapes.size(), {false, 0.f, 0.f});
    for (size_t i = 0; i < shapes.size(); ++i) {
        collisionStates[i].originalXSpeed = shapes[i].xSpeed;
        collisionStates[i].originalYSpeed = shapes[i].ySpeed;
    }

    while (window.isOpen()) {
        // Process window events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Update shapes and check for collisions
        for (size_t i = 0; i < sfmlShapes.size(); ++i) {
            auto& shape = sfmlShapes[i];
            auto& config = shapes[i];
            auto& state = collisionStates[i];

            // Update position based on speed
            sf::Vector2f position = shape->getPosition();
            position.x += config.xSpeed;
            position.y += config.ySpeed;

            // Reflect at window boundaries
            if (position.x < 0) {
                position.x = 0;
                config.xSpeed *= -1; // Reverse horizontal speed
            }
            if (position.x + (config.type == "rectangle" ? config.width : 2 * config.radius) > window.getSize().x) {
                position.x = window.getSize().x - (config.type == "rectangle" ? config.width : 2 * config.radius);
                config.xSpeed *= -1; // Reverse horizontal speed
            }
            if (position.y < 0) {
                position.y = 0;
                config.ySpeed *= -1; // Reverse vertical speed
            }
            if (position.y + (config.type == "rectangle" ? config.height : 2 * config.radius) > window.getSize().y) {
                position.y = window.getSize().y - (config.type == "rectangle" ? config.height : 2 * config.radius);
                config.ySpeed *= -1; // Reverse vertical speed
            }

            // Apply updated position to the shape
            shape->setPosition(position);

            // Reset shape color to its original
            shape->setFillColor(config.color);

            // Update text position to follow the shape
            auto bounds = shape->getGlobalBounds();
            auto& text = sfmlTexts[i];
            auto textBounds = text.getLocalBounds();
            text.setPosition(bounds.left + bounds.width / 2.f - textBounds.width / 2.f,
                            bounds.top + bounds.height / 2.f - textBounds.height / 2.f - textBounds.top);

            // Check for collisions with other shapes
            bool isColliding = false;
            for (size_t j = 0; j < sfmlShapes.size(); ++j) {
                if (i == j) continue; // Skip self-collision

                auto bounds1 = shape->getGlobalBounds();
                auto bounds2 = sfmlShapes[j]->getGlobalBounds();

                if (bounds1.intersects(bounds2)) {
                    isColliding = true;

                    auto& otherConfig = shapes[j];
                    auto& otherState = collisionStates[j];

                    if (!state.inCollision) {
                        // Save original speeds for both shapes
                        state.originalXSpeed = config.xSpeed;
                        state.originalYSpeed = config.ySpeed;

                        otherState.originalXSpeed = otherConfig.xSpeed;
                        otherState.originalYSpeed = otherConfig.ySpeed;

                        // Adjust speeds for both shapes
                        config.xSpeed *= -1.3f;
                        config.ySpeed *= -1.3f;

                        otherConfig.xSpeed *= -1.3f;
                        otherConfig.ySpeed *= -1.3f;

                        // Change colors for both shapes
                        shape->setFillColor(sf::Color::Red);
                        sfmlShapes[j]->setFillColor(sf::Color::Red);
                    }

                    // Mark both shapes as in collision
                    state.inCollision = true;
                    otherState.inCollision = true;
                }
            }

            // Reset speed when no longer colliding
            if (!isColliding && state.inCollision) {
                config.xSpeed = -state.originalXSpeed;
                config.ySpeed = -state.originalYSpeed;
                state.inCollision = false;
            }

            state.inCollision = isColliding;
        }

        // Render shapes and labels
        window.clear(sf::Color::Black);
        for (const auto& shape : sfmlShapes) {
            window.draw(*shape);
        }
        for (const auto& text : sfmlTexts) {
            window.draw(text);
        }
        window.display();
    }
    return 0;
}
