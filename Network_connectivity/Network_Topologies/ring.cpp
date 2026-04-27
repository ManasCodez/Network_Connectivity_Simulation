#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <string>

using namespace std;
    
int main() {
    sf::RenderWindow window(sf::VideoMode({1000, 800}), "Ring Network Topology Status");

    const int numNodes = 10;
    const float nodeRadius = 25.f;
    const float ringRadius = 280.f; 
    const sf::Vector2f center(500.f, 400.f);

    vector<sf::CircleShape> nodes(numNodes);
    vector<sf::Vector2f> positions(numNodes);
    vector<bool> active(numNodes, true);

    for (int i = 0; i < numNodes; i++) {
        float angle = i * (2 * 3.14159f / numNodes);
        positions[i] = {
            center.x + ringRadius * cos(angle),
            center.y + ringRadius * sin(angle)
        };

        nodes[i].setRadius(nodeRadius);
        nodes[i].setOrigin({nodeRadius, nodeRadius});
        nodes[i].setPosition(positions[i]);
        nodes[i].setFillColor(sf::Color::Green);
    }

    sf::Font font;
    if (!font.openFromFile("DejaVuSans.ttf")) cout << "Font load failed!" << endl;

    sf::Text infoText(font);
    infoText.setCharacterSize(24);
    infoText.setPosition({20.f, 20.f});

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mpos = window.mapPixelToCoords({mouseEvent->position.x, mouseEvent->position.y});

                    for (int i = 0; i < numNodes; i++) {
                        float dx = mpos.x - positions[i].x;
                        float dy = mpos.y - positions[i].y;
                        if (dx * dx + dy * dy <= nodeRadius * nodeRadius) {
                            active[i] = !active[i];
                            nodes[i].setFillColor(active[i] ? sf::Color::Green : sf::Color::Red);
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(25, 25, 25));

        // 🔹 Logic: Check if the ring is intact
        bool networkIntact = true;
        for (bool state : active) {
            if (!state) {
                networkIntact = false;
                break;
            }
        }

        // 🔹 Draw Edges
        for (int i = 0; i < numNodes; i++) {
            int next = (i + 1) % numNodes;
            if (active[i] && active[next]) {
                sf::Vertex line[] = {
                    { positions[i], sf::Color::Cyan },
                    { positions[next], sf::Color::Cyan }
                };
                window.draw(line, 2, sf::PrimitiveType::Lines);
            }
        }

        for (int i = 0; i < numNodes; i++) {
            window.draw(nodes[i]);
        }

        // 🔹 Status Text Update
        if (networkIntact) {
            infoText.setFillColor(sf::Color::Green);
            infoText.setString("Ring Status: OPERATIONAL\nAll nodes active. Data loop complete.");
        } else {
            infoText.setFillColor(sf::Color::Red);
            infoText.setString("Ring Status: DISRUPTED / OFFLINE\nBreak detected in the communication loop!");
        }

        window.draw(infoText);
        window.display();
    }

    return 0;
}