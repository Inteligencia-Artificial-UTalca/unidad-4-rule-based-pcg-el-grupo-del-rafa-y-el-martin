#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using Map = std::vector<std::vector<int>>;

void printMap(const Map& map) {
    std::cout << "--- Current Map ---" << std::endl;
    for (const auto& row : map) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-------------------" << std::endl;
}

Map cellularAutomata(const Map& currentMap, int W, int H, int R, double U) {
    Map newMap = currentMap;
    return newMap;
}

Map drunkAgent(const Map& currentMap, int W, int H, int J, int I, int roomSizeX, int roomSizeY,
               double probGenerateRoom, double probIncreaseRoom,
               double probChangeDirection, double probIncreaseChange,
               int& agentX, int& agentY) {

    Map newMap = currentMap;
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> chance(0.0, 1.0);
    std::uniform_int_distribution<int> dirDist(0, 3);

    std::vector<std::pair<int, int>> directions = {{0,1},{1,0},{0,-1},{-1,0}};
    double roomProb = probGenerateRoom;
    double dirProb = probChangeDirection;
    int dir = dirDist(rng);

    for (int j = 0; j < J; ++j) {
        for (int i = 0; i < I; ++i) {
            // Draw room
            if (chance(rng) < roomProb) {
                int roomWidth = std::uniform_int_distribution<int>(1, roomSizeX)(rng);
                int roomHeight = std::uniform_int_distribution<int>(1, roomSizeY)(rng);
                for (int dy = 0; dy < roomHeight; ++dy) {
                    for (int dx = 0; dx < roomWidth; ++dx) {
                        int nx = agentX + dx;
                        int ny = agentY + dy;
                        if (nx >= 0 && nx < H && ny >= 0 && ny < W) {
                            newMap[nx][ny] = 1;
                        }
                    }
                }
                roomProb = probGenerateRoom;
            } else {
                roomProb += probIncreaseRoom;
            }

            // Change direction
            if (chance(rng) < dirProb) {
                dir = dirDist(rng);
                dirProb = probChangeDirection;
            } else {
                dirProb += probIncreaseChange;
            }

            // Move agent
            agentX += directions[dir].first;
            agentY += directions[dir].second;

            if (agentX < 0) agentX = 0;
            if (agentX >= H) agentX = H - 1;
            if (agentY < 0) agentY = 0;
            if (agentY >= W) agentY = W - 1;

            newMap[agentX][agentY] = 1;
        }
    }

    return newMap;
}

int main() {
    std::cout << "--- CELLULAR AUTOMATA AND DRUNK AGENT SIMULATION ---" << std::endl;

    int mapRows = 10;
    int mapCols = 20;
    Map myMap(mapRows, std::vector<int>(mapCols, 0));

    int drunkAgentX = mapRows / 2;
    int drunkAgentY = mapCols / 2;

    std::cout << "\nInitial map state:" << std::endl;
    printMap(myMap);

    int numIterations = 5;

    int ca_W = mapCols;
    int ca_H = mapRows;
    int ca_R = 1;
    double ca_U = 0.5;

    int da_W = mapCols;
    int da_H = mapRows;
    int da_J = 5;
    int da_I = 10;
    int da_roomSizeX = 5;
    int da_roomSizeY = 3;
    double da_probGenerateRoom = 0.1;
    double da_probIncreaseRoom = 0.05;
    double da_probChangeDirection = 0.2;
    double da_probIncreaseChange = 0.03;

    for (int iteration = 0; iteration < numIterations; ++iteration) {
        std::cout << "\n--- Iteration " << iteration + 1 << " ---" << std::endl;

        myMap = cellularAutomata(myMap, ca_W, ca_H, ca_R, ca_U);
        myMap = drunkAgent(myMap, da_W, da_H, da_J, da_I, da_roomSizeX, da_roomSizeY,
                           da_probGenerateRoom, da_probIncreaseRoom,
                           da_probChangeDirection, da_probIncreaseChange,
                           drunkAgentX, drunkAgentY);

        printMap(myMap);
    }

    std::cout << "\n--- Simulation Finished ---" << std::endl;
    return 0;
}
