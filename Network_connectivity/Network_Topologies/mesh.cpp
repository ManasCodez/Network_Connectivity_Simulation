#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

// Connectivity Check using Adjacency Matrix
bool isConnectedMatrix(int n, const vector<vector<bool>> &adjMatrix, const vector<bool> &active)
{
    vector<bool> visited(n, false);
    int start = -1;

    for (int i = 0; i < n; i++)
    {
        if (active[i])
        {
            start = i;
            break;
        }
    }

    if (start == -1)
        return true;

    vector<int> stack;
    stack.push_back(start);
    visited[start] = true;

    while (!stack.empty())
    {
        int curr = stack.back();
        stack.pop_back();

        for (int i = 0; i < n; i++)
        {
            if (adjMatrix[curr][i] && !visited[i])
            {
                visited[i] = true;
                stack.push_back(i);
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (active[i] && !visited[i])
            return false;
    }
    return true;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({1200, 1000}), "Full Mesh Topology (Complete Graph)");

    int lastNode = -1;
    int lastEdges = 0;
    bool wasRemoved = false;

    const int rows = 3;
    const int cols = 3;
    const int n = rows * cols; 
    const float spacing = 150.f;
    const float radius = 20.f;

    vector<sf::CircleShape> nodes;
    vector<sf::Vector2f> centers;
    vector<bool> active(n, true);

    vector<vector<bool>> adjMatrix(n, vector<bool>(n, false));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            sf::CircleShape node(radius);
            node.setFillColor(sf::Color::Green);
            float x = 200.f + j * spacing;
            float y = 150.f + i * spacing;
            node.setPosition({x, y});
            nodes.push_back(node);
            centers.push_back({x + radius, y + radius});
        }
    }

    for (int u = 0; u < n; u++)
    {
        for (int v = 0; v < n; v++)
        {
            if (u != v) 
            {
                adjMatrix[u][v] = true;
            }
        }
    }

    sf::Font font;
    if (!font.openFromFile("DejaVuSans.ttf"))
        cout << "Font load failed!" << endl;

    sf::Text infoText(font);
    infoText.setCharacterSize(24);
    infoText.setFillColor(sf::Color::Yellow);
    infoText.setPosition({10.f, 10.f});

    sf::Color lineColor(255, 255, 255, 100); 

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
                    // 🔹 FIX 1: Removed the (float) cast. Passed as an sf::Vector2i using braces
                    sf::Vector2f mpos = window.mapPixelToCoords({mouseEvent->position.x, mouseEvent->position.y});

                    for (int i = 0; i < n; i++)
                    {
                        float dx = mpos.x - centers[i].x;
                        float dy = mpos.y - centers[i].y;

                        if (dx * dx + dy * dy <= (radius + 5) * (radius + 5))
                        {
                            lastNode = i;
                            lastEdges = 0;

                            if (active[i])
                            {
                                for (int j = 0; j < n; j++)
                                {
                                    if (adjMatrix[i][j])
                                    {
                                        lastEdges++;
                                        adjMatrix[i][j] = false;
                                        adjMatrix[j][i] = false;
                                    }
                                }
                                active[i] = false;
                                nodes[i].setFillColor(sf::Color::Red);
                                wasRemoved = true;
                            }
                            else
                            {
                                active[i] = true;
                                nodes[i].setFillColor(sf::Color::Green);
                                wasRemoved = false;

                                for (int j = 0; j < n; j++)
                                {
                                    if (i != j && active[j]) 
                                    {
                                        adjMatrix[i][j] = true;
                                        adjMatrix[j][i] = true;
                                        lastEdges++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        window.clear();

        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (adjMatrix[i][j])
                {
                    // 🔹 FIX 2: Replaced parentheses () with curly braces {} for sf::Vertex
                    sf::Vertex line[] = {
                        sf::Vertex{centers[i], lineColor}, 
                        sf::Vertex{centers[j], lineColor}
                    };
                    window.draw(line, 2, sf::PrimitiveType::Lines);
                }
            }
        }

        for (auto &node : nodes)
            window.draw(node);

        if (lastNode != -1)
        {
            string status = wasRemoved ? "Removed" : "Restored";
            bool meshIntact = isConnectedMatrix(n, adjMatrix, active);
            string health = meshIntact ? " (Mesh: Intact)" : " (Mesh: Broken)";

            infoText.setString("Node: " + to_string(lastNode) + " | " + status + " Edges: " + to_string(lastEdges) + health);
        }
        else
        {
            infoText.setString("Full Mesh Topology: Click a node to toggle");
        }

        window.draw(infoText);
        window.display();
    }
    return 0;
}