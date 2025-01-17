#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

int main() {
    // Create SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "ImGui + SFML Demo");
    window.setFramerateLimit(60);

    // Initialize ImGui-SFML
    [[maybe_unused]] bool imguiInitialized = ImGui::SFML::Init(window);

    sf::Clock deltaClock; // For tracking frame time

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Pass events to ImGui
            ImGui::SFML::ProcessEvent(event);
        }

        // Start a new ImGui frame
        ImGui::SFML::Update(window, deltaClock.restart());

        // Show ImGui demo window
        ImGui::ShowDemoWindow();

        // Clear the window
        window.clear();

        // Render ImGui
        ImGui::SFML::Render(window);

        // Display on screen
        window.display();
    }

    // Cleanup
    ImGui::SFML::Shutdown();
    return 0;
}
