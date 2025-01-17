#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

int main() {
    // Create a window using SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Beginner-Friendly ImGui Example");
    window.setFramerateLimit(60);

    // Initialize ImGui-SFML
    [[maybe_unused]] bool imguiInitialized = ImGui::SFML::Init(window);


    // Clock for SFML/ImGui updates
    sf::Clock deltaClock;

    // Variables for the GUI
    bool showText = false;
    float sliderValue = 0.5f;
    int comboIndex = 0;
    const char* items[] = { "Option 1", "Option 2", "Option 3" }; // By default imgui wants a c-style char array
    std::vector<const char*> items2; // To make it easier we could use a vector of const char


    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Handle window close event
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Pass events to ImGui
            ImGui::SFML::ProcessEvent(event);
        }

        // Start ImGui frame
        ImGui::SFML::Update(window, deltaClock.restart());

        // Create an ImGui window
        ImGui::Begin("Control Panel");
        

        // Add a button
        if (ImGui::Button("Toggle Text")) {
            showText = !showText; // Toggle text visibility
        }

        // Add a slider
        ImGui::SliderFloat("Adjust Value", &sliderValue, 0.0f, 1.0f);

        // Add a dropdown menu

        // Approach 1: use c-style array
        if (ImGui::BeginCombo("Options", items[comboIndex])) {
            for (int i = 0; i < IM_ARRAYSIZE(items); ++i) {
                if (ImGui::Selectable(items[i], comboIndex == i)) {
                    comboIndex = i;
                }
            }
            ImGui::EndCombo();
        }
        // Approach 2: use vectors
        // if (ImGui::BeginCombo("Options"), items[comboIndex]) {
        //     for (int i=0, i< items.size(); i++) {
        //         if (ImGui::Selectable(items[i], comboIndex==i)) {
        //             comboIndex = i;
        //         }
        //     }
        // }

        // Show text conditionally
        if (showText) {
            ImGui::Text("Hello, ImGui!");
        }

        // End the ImGui window
        ImGui::End();

        // Clear the window
        window.clear(sf::Color(50, 50, 50)); // A blank dark gray background

        // Render ImGui on the window
        ImGui::SFML::Render(window);

        // Display everything
        window.display();
    }

    // Cleanup ImGui-SFML
    ImGui::SFML::Shutdown();

    return 0;
}
