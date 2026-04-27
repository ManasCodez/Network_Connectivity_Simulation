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

    // Find the first active node to start traversal
    for (int i = 0; i < n; i++)
    {
        if (active[i])
        {
            start = i;
            break;
        }
    }

    // If no nodes are active, it's technically "connected" (empty graph)
    if (start == -1)
        return true;

    // Iterative DFS using the matrix
    vector<int> stack;
    stack.push_back(start);
    visited[start] = true;

    while (!stack.empty())
    {
        int curr = stack.back();
        stack.pop_back();

        // Check all potential neighbors in the matrix
        for (int i = 0; i < n; i++)
        {
            if (adjMatrix[curr][i] && !visited[i])
            {
                visited[i] = true;
                stack.push_back(i);
            }
        }
    }

    // Verify all active nodes were visited
    for (int i = 0; i < n; i++)
    {
        if (active[i] && !visited[i])
            return false;
    }
    return true;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({1400, 900}), "Real-Time Mesh Connectivity");

    int lastNode = -1;
    int lastEdges = 0;
    bool wasRemoved = false;

    const int rows = 5;
    const int cols = 7;
    const int n = rows * cols; // Total nodes
    const float spacing = 150.f;
    const float radius = 20.f;

    vector<sf::CircleShape> nodes;
    vector<sf::Vector2f> centers;
    vector<bool> active(n, true);

    // Adjacency Matrix to store connection status between any node U and V
    vector<vector<bool>> adjMatrix(n, vector<bool>(n, false));

    // Initialize nodes and default connections in the matrix
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

            int u = i * cols + j;

            // 8-way directional connections
            vector<pair<int, int>> dirs = {
                {i, j + 1}, {i, j - 1}, {i + 1, j}, {i - 1, j}, {i - 1, j - 1}, {i - 1, j + 1}, {i + 1, j - 1}, {i + 1, j + 1}};

            for (auto [nr, nc] : dirs)
            {
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols)
                {
                    int v = nr * cols + nc;
                    adjMatrix[u][v] = true;
                }
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
                    sf::Vector2f mpos = window.mapPixelToCoords({(float)mouseEvent->position.x, (float)mouseEvent->position.y});

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
                                // 🔹 Removing Node: Clear connections in Matrix
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
                                // 🔹 Restoring Node: Rebuild connections in Matrix based on grid logic
                                active[i] = true;
                                nodes[i].setFillColor(sf::Color::Green);
                                wasRemoved = false;

                                int r = i / cols;
                                int c = i % cols;
                                vector<pair<int, int>> dirs = {
                                    {r, c + 1}, {r, c - 1}, {r + 1, c}, {r - 1, c}, {r - 1, c - 1}, {r - 1, c + 1}, {r + 1, c - 1}, {r + 1, c + 1}};

                                for (auto [nr, nc] : dirs)
                                {
                                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols)
                                    {
                                        int j = nr * cols + nc;
                                        if (active[j]) // Connect only if the neighbor is active
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
        }

        window.clear();

        // 🔹 Draw Edges strictly based on the Adjacency Matrix
        for (int i = 0; i < n; i++)
        {
            // Start j from i + 1 to avoid drawing overlapping double lines (since graph is undirected)
            for (int j = i + 1; j < n; j++)
            {
                if (adjMatrix[i][j])
                {
                    sf::Vertex line[] = {sf::Vertex{centers[i]}, sf::Vertex{centers[j]}};
                    window.draw(line, 2, sf::PrimitiveType::Lines);
                }
            }
        }

        for (auto &node : nodes)
            window.draw(node);

        // Real-Time Text Update
        if (lastNode != -1)
        {
            string status = wasRemoved ? "Removed" : "Restored";

            // Optional: Call isConnectedMatrix to display global mesh health
            bool meshIntact = isConnectedMatrix(n, adjMatrix, active);
            string health = meshIntact ? " (Mesh: Intact)" : " (Mesh: Broken)";

            infoText.setString("Node: " + to_string(lastNode) + " | " + status + " Edges: " + to_string(lastEdges) + health);
        }
        else
        {
            infoText.setString("Mesh Connectivity: Click a node to toggle");
        }

        window.draw(infoText);
        window.display();
    }
    return 0;
}