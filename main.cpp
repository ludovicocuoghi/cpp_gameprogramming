#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
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
            iss >> windowWidth >> windowHeight; // Set window size
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

    // Initialize ImGui-SFML
    [[maybe_unused]] bool imguiInitialized = ImGui::SFML::Init(window);

    // Create SFML shapes and their text labels
    std::vector<std::unique_ptr<sf::Shape>> sfmlShapes;
    std::vector<sf::Text> sfmlTexts;

    for (const auto& shape : shapes) {
        sf::Text text;
        text.setFont(font);
        text.setString(shape.name);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(24);

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

        sfmlTexts.push_back(text);
    }

    // Initialize collision states for all shapes
    std::vector<CollisionState> collisionStates(shapes.size(), {false, 0.f, 0.f});
    for (size_t i = 0; i < shapes.size(); ++i) {
        collisionStates[i].originalXSpeed = shapes[i].xSpeed;
        collisionStates[i].originalYSpeed = shapes[i].ySpeed;
    }

    // Selected shape index for the GUI
    int selectedShapeIndex = -1;

    sf::Clock clock;
    while (window.isOpen()) {
        // Process window events
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event); // Pass events to ImGui
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, clock.restart()); // Update ImGui

        // GUI: Dropdown for selecting a shape
        ImGui::Begin("Shape Editor");
        if (ImGui::BeginCombo("Select Shape", selectedShapeIndex >= 0 ? shapes[selectedShapeIndex].name.c_str() : "None")) {
            for (size_t i = 0; i < shapes.size(); ++i) {
                if (ImGui::Selectable(shapes[i].name.c_str(), selectedShapeIndex == (int)i)) {
                    selectedShapeIndex = (int)i;
                }
            }
            ImGui::EndCombo();
        }

        if (selectedShapeIndex >= 0 && selectedShapeIndex < (int)shapes.size()) {
            auto& selectedConfig = shapes[selectedShapeIndex];
            auto& selectedShape = sfmlShapes[selectedShapeIndex];

            ImGui::SliderFloat("X Speed", &selectedConfig.xSpeed, -10.0f, 10.0f);
            ImGui::SliderFloat("Y Speed", &selectedConfig.ySpeed, -10.0f, 10.0f);
            if (selectedConfig.type == "rectangle") {
                ImGui::SliderFloat("Width", &selectedConfig.width, 10.0f, 200.0f);
                ImGui::SliderFloat("Height", &selectedConfig.height, 10.0f, 200.0f);
                dynamic_cast<sf::RectangleShape*>(selectedShape.get())->setSize(
                    sf::Vector2f(selectedConfig.width, selectedConfig.height));
            } else if (selectedConfig.type == "circle") {
                ImGui::SliderFloat("Radius", &selectedConfig.radius, 5.0f, 100.0f);
                dynamic_cast<sf::CircleShape*>(selectedShape.get())->setRadius(selectedConfig.radius);
            }
            // Temporary color array for ImGui
            float tempColor[3] = {
                selectedConfig.color.r / 255.0f, // Normalize the color to 0.0 - 1.0 range
                selectedConfig.color.g / 255.0f,
                selectedConfig.color.b / 255.0f
            };

            // Use ImGui to edit the color
            if (ImGui::ColorEdit3("Color", tempColor)) {
                // De-normalize back to 0-255 range and update the shape color
                selectedConfig.color.r = static_cast<sf::Uint8>(tempColor[0] * 255.0f);
                selectedConfig.color.g = static_cast<sf::Uint8>(tempColor[1] * 255.0f);
                selectedConfig.color.b = static_cast<sf::Uint8>(tempColor[2] * 255.0f);
                selectedShape->setFillColor(selectedConfig.color);
            }
        }
        ImGui::End();

        // Update shapes and handle collisions
        for (size_t i = 0; i < sfmlShapes.size(); ++i) {
            auto& shape = sfmlShapes[i];
            auto& config = shapes[i];
            auto& state = collisionStates[i];

            sf::Vector2f position = shape->getPosition();
            position.x += config.xSpeed;
            position.y += config.ySpeed;

            if (position.x < 0 || position.x + (config.type == "rectangle" ? config.width : 2 * config.radius) > window.getSize().x) {
                config.xSpeed *= -1;
                position.x = std::clamp(position.x, 0.f, window.getSize().x - (config.type == "rectangle" ? config.width : 2 * config.radius));
            }
            if (position.y < 0 || position.y + (config.type == "rectangle" ? config.height : 2 * config.radius) > window.getSize().y) {
                config.ySpeed *= -1;
                position.y = std::clamp(position.y, 0.f, window.getSize().y - (config.type == "rectangle" ? config.height : 2 * config.radius));
            }
            shape->setPosition(position);

            // Update the text position to follow the shape
            auto bounds = shape->getGlobalBounds();
            auto& text = sfmlTexts[i];
            auto textBounds = text.getLocalBounds();
            text.setPosition(bounds.left + bounds.width / 2.f - textBounds.width / 2.f,
                             bounds.top + bounds.height / 2.f - textBounds.height / 2.f - textBounds.top);

            bool isColliding = false;
            for (size_t j = 0; j < sfmlShapes.size(); ++j) {
                if (i == j) continue;
                auto bounds1 = shape->getGlobalBounds();
                auto bounds2 = sfmlShapes[j]->getGlobalBounds();

                if (bounds1.intersects(bounds2)) {
                    isColliding = true;
                    auto& otherConfig = shapes[j];
                    auto& otherState = collisionStates[j];

                    if (!state.inCollision) {
                        state.originalXSpeed = config.xSpeed;
                        state.originalYSpeed = config.ySpeed;

                        otherState.originalXSpeed = otherConfig.xSpeed;
                        otherState.originalYSpeed = otherConfig.ySpeed;

                        config.xSpeed *= -1.3f;
                        config.ySpeed *= -1.3f;

                        otherConfig.xSpeed *= -1.3f;
                        otherConfig.ySpeed *= -1.3f;

                        shape->setFillColor(sf::Color::Red);
                        sfmlShapes[j]->setFillColor(sf::Color::Red);
                    }

                    state.inCollision = true;
                    otherState.inCollision = true;
                }
            }

            if (!isColliding && state.inCollision) {
                config.xSpeed = -state.originalXSpeed;
                config.ySpeed = -state.originalYSpeed;
                state.inCollision = false;
                shape->setFillColor(config.color);
            }
            state.inCollision = isColliding;
        }

        // Render everything
        window.clear(sf::Color::Black);
        for (const auto& shape : sfmlShapes) {
            window.draw(*shape);
        }
        for (const auto& text : sfmlTexts) {
            window.draw(text);
        }
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
