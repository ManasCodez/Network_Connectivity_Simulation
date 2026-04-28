#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

// 🔹 Calculates shortest distance from a point P to a line segment AB
float getDistToLine(sf::Vector2f p, sf::Vector2f a, sf::Vector2f b)
{
    float l2 = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
    if (l2 == 0.0f) return sqrt((p.x - a.x) * (p.x - a.x) + (p.y - a.y) * (p.y - a.y));
    
    // Find the projection point "t" along the line
    float t = max(0.0f, min(1.0f, ((p.x - a.x) * (b.x - a.x) + (p.y - a.y) * (b.y - a.y)) / l2));
    sf::Vector2f proj = {a.x + t * (b.x - a.x), a.y + t * (b.y - a.y)};
    
    // Return distance from cursor to projected point
    return sqrt((p.x - proj.x) * (p.x - proj.x) + (p.y - proj.y) * (p.y - proj.y));
}

// 🔹 BFS to find connected components and color disconnected nodes
void updateNetworkStatus(int n, const vector<vector<bool>>& adjMatrix, vector<sf::CircleShape>& nodes, int& disconnectedCount)
{
    vector<int> compId(n, -1);
    vector<int> compSize(n, 0);
    int currentComp = 0;

    // 1. Group nodes into connected components
    for (int i = 0; i < n; i++)
    {
        if (compId[i] == -1)
        {
            vector<int> q;
            q.push_back(i);
            compId[i] = currentComp;
            int size = 1;
            int head = 0;

            while (head < q.size())
            {
                int u = q[head++];
                for (int v = 0; v < n; v++)
                {
                    if (adjMatrix[u][v] && compId[v] == -1)
                    {
                        compId[v] = currentComp;
                        q.push_back(v);
                        size++;
                    }
                }
            }
            compSize[currentComp] = size;
            currentComp++;
        }
    }

    // 2. Find the size of the largest "Main" network
    int maxCompSize = 0;
    for (int i = 0; i < currentComp; i++)
    {
        if (compSize[i] > maxCompSize)
            maxCompSize = compSize[i];
    }

    // 3. Color the nodes based on if they are in the main network or not
    disconnectedCount = 0;
    for (int i = 0; i < n; i++)
    {
        // If it's part of the largest group (and not completely isolated alone)
        if (compSize[compId[i]] == maxCompSize && maxCompSize > 1)
        {
            nodes[i].setFillColor(sf::Color::Green); // Connected
        }
        else
        {
            nodes[i].setFillColor(sf::Color::Red);   // Disconnected
            disconnectedCount++;
        }
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode({1200, 1000}), "Click Edges to Disconnect");

    int removedEdgesCount = 0;
    int disconnectedNodesCount = 0;

    const int rows = 3;
    const int cols = 3;
    const int n = rows * cols; 
    const float spacing = 200.f; // Spaced out slightly more to make edges easier to click
    const float radius = 20.f;

    vector<sf::CircleShape> nodes;
    vector<sf::Vector2f> centers;
    vector<vector<bool>> adjMatrix(n, vector<bool>(n, false));

    // Initialize Nodes
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

    // Initialize Full Mesh
    for (int u = 0; u < n; u++)
    {
        for (int v = 0; v < n; v++)
        {
            if (u != v) adjMatrix[u][v] = true;
        }
    }

    // Font Setup
    sf::Font font;
    if (!font.openFromFile("DejaVuSans.ttf")) cout << "Font load failed!" << endl;

    sf::Text infoText(font);
    infoText.setCharacterSize(24);
    infoText.setFillColor(sf::Color::Yellow);
    infoText.setPosition({10.f, 10.f});

    // 🔹 Restore Button UI
    sf::RectangleShape restoreBtn(sf::Vector2f{200.f, 50.f});
    restoreBtn.setPosition({900.f, 20.f});
    restoreBtn.setFillColor(sf::Color(70, 130, 180)); // Steel Blue

    sf::Text btnText(font);
    btnText.setString("Restore Edges");
    btnText.setCharacterSize(20);
    btnText.setFillColor(sf::Color::White);
    btnText.setPosition({925.f, 32.f});

    sf::Color lineColor(255, 255, 255, 150); 

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

                    // 1. Check if Restore Button was clicked
                    if (restoreBtn.getGlobalBounds().contains(mpos))
                    {
                        for (int u = 0; u < n; u++) {
                            for (int v = 0; v < n; v++) {
                                if (u != v) adjMatrix[u][v] = true;
                            }
                        }
                        removedEdgesCount = 0;
                    }
                    else
                    {
                        // 2. Check if an Edge was clicked
                        float clickThreshold = 8.0f; // How close cursor needs to be to a line
                        float minDist = clickThreshold;
                        int clickedU = -1, clickedV = -1;

                        for (int i = 0; i < n; i++)
                        {
                            for (int j = i + 1; j < n; j++)
                            {
                                if (adjMatrix[i][j]) // Only check active edges
                                {
                                    float dist = getDistToLine(mpos, centers[i], centers[j]);
                                    if (dist < minDist)
                                    {
                                        minDist = dist;
                                        clickedU = i;
                                        clickedV = j;
                                    }
                                }
                            }
                        }

                        // If an edge was close enough, remove it
                        if (clickedU != -1)
                        {
                            adjMatrix[clickedU][clickedV] = false;
                            adjMatrix[clickedV][clickedU] = false;
                            removedEdgesCount++;
                        }
                    }
                }
            }
        }

        // Update the network status and node colors mathematically based on the Matrix
        updateNetworkStatus(n, adjMatrix, nodes, disconnectedNodesCount);

        window.clear();

        // Draw Edges
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (adjMatrix[i][j])
                {
                    sf::Vertex line[] = {
                        sf::Vertex{centers[i], lineColor}, 
                        sf::Vertex{centers[j], lineColor}
                    };
                    window.draw(line, 2, sf::PrimitiveType::Lines);
                }
            }
        }

        // Draw Nodes
        for (auto &node : nodes)
            window.draw(node);

        // Draw UI
        infoText.setString("Removed Edges: " + to_string(removedEdgesCount) + 
                           "\nDisconnected Vertices: " + to_string(disconnectedNodesCount));
        
        window.draw(restoreBtn);
        window.draw(btnText);
        window.draw(infoText);
        
        window.display();
    }
    return 0;
}