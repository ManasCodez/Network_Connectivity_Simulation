#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

// Connectivity Check for Star Topology
// In a star, connectivity exists if the Hub is active AND connected to active peripherals.
bool isStarConnected(int hubIndex, const vector<bool> &active)
{
    if (!active[hubIndex])
        return false; // If hub is down, nothing is connected
    return true;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({1000, 800}), "Star Network Topology Connectivity");

    const int numNodes = 8; // Total nodes including the hub
    const int hubIndex = 0;
    const float radius = 25.f;
    const float networkRadius = 250.f; // Distance of peripherals from center
    const sf::Vector2f center(500.f, 400.f);

    vector<sf::CircleShape> nodes(numNodes);
    vector<sf::Vector2f> positions(numNodes);
    vector<bool> active(numNodes, true);

    // Initialize Hub and Peripheral positions
    for (int i = 0; i < numNodes; i++)
    {
        if (i == hubIndex)
        {
            positions[i] = center;
        }
        else
        {
            // Distribute peripherals in a circle around the hub
            float angle = (i - 1) * (2 * 3.14159f / (numNodes - 1));
            positions[i] = {
                center.x + networkRadius * cos(angle),
                center.y + networkRadius * sin(angle)};
        }

        nodes[i].setRadius(radius);
        nodes[i].setOrigin({radius, radius}); // Center the origin for easier drawing
        nodes[i].setPosition(positions[i]);
        nodes[i].setFillColor(sf::Color::Green);
    }

    sf::Font font;
    if (!font.openFromFile("DejaVuSans.ttf"))
        cout << "Font load failed!" << endl;

    sf::Text infoText(font);
    infoText.setCharacterSize(22);
    infoText.setFillColor(sf::Color::Cyan);
    infoText.setPosition({10.f, 10.f});

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto *mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseEvent->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mpos = window.mapPixelToCoords({mouseEvent->position.x, mouseEvent->position.y});

                    for (int i = 0; i < numNodes; i++)
                    {
                        float dx = mpos.x - positions[i].x;
                        float dy = mpos.y - positions[i].y;

                        if (dx * dx + dy * dy <= radius * radius)
                        {
                            active[i] = !active[i];
                            nodes[i].setFillColor(active[i] ? sf::Color::Green : sf::Color::Red);
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(20, 20, 20)); // Dark background

        //  Draw Edges (Only Hub to Active Peripherals)
        if (active[hubIndex])
        {
            for (int i = 1; i < numNodes; i++)
            {
                if (active[i])
                {
                    sf::Vertex line[] = {
                        {positions[hubIndex], sf::Color::White},
                        {positions[i], sf::Color::White}};

                    window.draw(line, 2, sf::PrimitiveType::Lines);
                }
            }
        }

        // 🔹 Draw Nodes
        for (int i = 0; i < numNodes; i++)
        {
            window.draw(nodes[i]);
        }

        // 🔹 UI Logic
        string status;
        if (!active[hubIndex])
        {
            status = "CRITICAL: HUB OFFLINE - Network Collapsed";
            infoText.setFillColor(sf::Color::Red);
        }
        else
        {
            status = "Hub Online | Network Functional";
            infoText.setFillColor(sf::Color::Green);
        }
        infoText.setString("Star Topology: " + status + "\nClick nodes to toggle state.");

        window.draw(infoText);
        window.display();
    }

    return 0;
}