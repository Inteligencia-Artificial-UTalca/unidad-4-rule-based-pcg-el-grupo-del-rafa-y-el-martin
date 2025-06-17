/*
 * Rule-Based Procedural Content Generation (PCG)
 * Implementación de Drunk Agent para generación de mazmorras
 * 
 * El Drunk Agent es un algoritmo de generación procedural que simula
 * un agente que se mueve aleatoriamente por una grilla, creando pasillos
 * y habitaciones para formar una mazmorra.
 * 
 * Parámetros del algoritmo:
 * - W, H: Dimensiones del mapa (ancho x alto)
 * - J: Número de movimientos que realizará el agente
 * - I: Número de pasos por cada movimiento
 * - A: Probabilidad inicial de generar habitación (valor inicial)
 * - B: Incremento de probabilidad cuando no se genera habitación
 * - C: Probabilidad inicial de cambiar dirección (valor inicial)
 * - D: Incremento de probabilidad cuando no se cambia dirección
 * - roomSizeX, roomSizeY: Dimensiones máximas de las habitaciones
 */

#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using Map = std::vector<std::vector<int>>;

void printMap(const Map& map) {
    std::cout << "--- Current Map ---" << std::endl;
    for (const auto& row : map) {
        for (int cell : row) {
            if (cell == 0) {
                std::cout << ". ";  // Espacios vacíos
            } else {
                std::cout << "# ";  // Pasillos y habitaciones
            }
        }
        std::cout << std::endl;
    }
    std::cout << "-------------------" << std::endl;
}

// Placeholder para el autómata celular (no implementado aquí)
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
    
    // Posición inicial aleatoria si es la primera vez
    if (agentX == -1 || agentY == -1) {
        std::uniform_int_distribution<int> xDist(0, H - 1);
        std::uniform_int_distribution<int> yDist(0, W - 1);
        agentX = xDist(rng);
        agentY = yDist(rng);
    }

    // Direcciones: Norte, Este, Sur, Oeste
    std::vector<std::pair<int, int>> directions = {{-1,0},{0,1},{1,0},{0,-1}};
    double roomProb = probGenerateRoom;
    double dirProb = probChangeDirection;
    int dir = dirDist(rng);

    std::cout << "Drunk Agent starting at position (" << agentX << ", " << agentY << ")" << std::endl;

    for (int j = 0; j < J; ++j) {
        std::cout << "Movement " << j + 1 << " of " << J << std::endl;
        
        // Al final de cada movimiento, intentar generar habitación
        if (chance(rng) < roomProb) {
            std::cout << "  Generating room at (" << agentX << ", " << agentY << ")" << std::endl;
            
            // Generar habitación centrada en el agente
            int startX = std::max(0, agentX - roomSizeX / 2);
            int startY = std::max(0, agentY - roomSizeY / 2);
            int endX = std::min(H - 1, startX + roomSizeX - 1);
            int endY = std::min(W - 1, startY + roomSizeY - 1);

            for (int x = startX; x <= endX; ++x) {
                for (int y = startY; y <= endY; ++y) {
                    newMap[x][y] = 1;
                }
            }

            roomProb = probGenerateRoom;  // Resetear probabilidad
        } else {
            roomProb = std::min(1.0, roomProb + probIncreaseRoom);  // Aumentar probabilidad
        }

        // Decidir cambio de dirección
        if (chance(rng) < dirProb) {
            dir = dirDist(rng);
            dirProb = probChangeDirection;  // Resetear probabilidad
            std::cout << "  Agent changed direction" << std::endl;
        } else {
            dirProb = std::min(1.0, dirProb + probIncreaseChange);  // Aumentar probabilidad
        }

        // Realizar I pasos en la dirección actual
        for (int i = 0; i < I; ++i) {
            int nextX = agentX + directions[dir].first;
            int nextY = agentY + directions[dir].second;

            // Verificar límites del mapa
            if (nextX < 0 || nextX >= H || nextY < 0 || nextY >= W) {
                // Cambiar dirección cuando se sale del mapa
                dir = dirDist(rng);
                std::cout << "  Agent hit boundary, changing direction" << std::endl;
                continue;
            }

            // Mover agente y marcar el camino
            agentX = nextX;
            agentY = nextY;
            newMap[agentX][agentY] = 1;  // Marcar pasillo
        }
    }

    std::cout << "Drunk Agent finished at position (" << agentX << ", " << agentY << ")" << std::endl;
    return newMap;
}

// Versión mejorada del Drunk Agent con mejor control de probabilidades
Map enhancedDrunkAgent(Map& currentMap, int W, int H, int J, int I, int roomSizeX, int roomSizeY,
                      double A, double B, double C, double D) {
    
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> chance(0.0, 1.0);
    std::uniform_int_distribution<int> dirDist(0, 3);
    std::uniform_int_distribution<int> xDist(0, H - 1);
    std::uniform_int_distribution<int> yDist(0, W - 1);

    // Posición inicial aleatoria
    int agentX = xDist(rng);
    int agentY = yDist(rng);
    
    // Direcciones: Norte, Este, Sur, Oeste
    std::vector<std::pair<int, int>> directions = {{-1,0},{0,1},{1,0},{0,-1}};
    std::vector<std::string> dirNames = {"North", "East", "South", "West"};
    
    double roomProb = A;        // Probabilidad actual de generar habitación
    double dirProb = C;         // Probabilidad actual de cambiar dirección
    int currentDir = dirDist(rng);

    std::cout << "\n=== Enhanced Drunk Agent Starting ===" << std::endl;
    std::cout << "Initial position: (" << agentX << ", " << agentY << ")" << std::endl;
    std::cout << "Parameters: J=" << J << ", I=" << I << ", Room=" << roomSizeX << "x" << roomSizeY << std::endl;
    std::cout << "Probabilities: A=" << A << ", B=" << B << ", C=" << C << ", D=" << D << std::endl;

    for (int j = 0; j < J; ++j) {
        std::cout << "\n--- Movement Phase " << (j + 1) << "/" << J << " ---" << std::endl;
        std::cout << "Current room probability: " << roomProb << std::endl;
        std::cout << "Current direction change probability: " << dirProb << std::endl;
        
        // Decidir si cambiar dirección al inicio de cada movimiento
        if (chance(rng) < dirProb) {
            int newDir = dirDist(rng);
            std::cout << "Direction changed from " << dirNames[currentDir] 
                     << " to " << dirNames[newDir] << std::endl;
            currentDir = newDir;
            dirProb = C;  // Resetear probabilidad de cambio de dirección
        } else {
            dirProb = std::min(1.0, dirProb + D);  // Aumentar probabilidad
            std::cout << "Direction maintained: " << dirNames[currentDir] << std::endl;
        }

        // Realizar I pasos en la dirección actual
        for (int i = 0; i < I; ++i) {
            int nextX = agentX + directions[currentDir].first;
            int nextY = agentY + directions[currentDir].second;

            // Verificar límites y detener si se sale del mapa
            if (nextX < 0 || nextX >= H || nextY < 0 || nextY >= W) {
                std::cout << "  Hit boundary at step " << (i + 1) << ", stopping movement" << std::endl;
                // Cambiar dirección cuando se sale del mapa
                currentDir = dirDist(rng);
                break;
            }

            // Mover agente y marcar el camino (pasillo)
            agentX = nextX;
            agentY = nextY;
            currentMap[agentX][agentY] = 1;
        }

        std::cout << "Agent position after movement: (" << agentX << ", " << agentY << ")" << std::endl;

        // Al final del movimiento, intentar generar habitación
        if (chance(rng) < roomProb) {
            std::cout << "Generating room at (" << agentX << ", " << agentY << ")" << std::endl;
            
            // Calcular límites de la habitación centrada en el agente
            int startX = std::max(0, agentX - roomSizeY / 2);
            int startY = std::max(0, agentY - roomSizeX / 2);
            int endX = std::min(H - 1, agentX + (roomSizeY - 1) / 2);
            int endY = std::min(W - 1, agentY + (roomSizeX - 1) / 2);

            // Asegurar que la habitación se genere incluso cerca del borde
            if (endX - startX + 1 < roomSizeY || endY - startY + 1 < roomSizeX) {
                // Ajustar para que la habitación tenga el tamaño mínimo
                if (endX - startX + 1 < roomSizeY) {
                    if (startX > 0) startX = std::max(0, endX - roomSizeY + 1);
                    else endX = std::min(H - 1, startX + roomSizeY - 1);
                }
                if (endY - startY + 1 < roomSizeX) {
                    if (startY > 0) startY = std::max(0, endY - roomSizeX + 1);
                    else endY = std::min(W - 1, startY + roomSizeX - 1);
                }
            }

            // Generar la habitación
            for (int x = startX; x <= endX; ++x) {
                for (int y = startY; y <= endY; ++y) {
                    currentMap[x][y] = 1;
                }
            }

            roomProb = A;  // Resetear probabilidad de habitación
            std::cout << "Room generated: " << (endX - startX + 1) << "x" << (endY - startY + 1) << std::endl;
        } else {
            roomProb = std::min(1.0, roomProb + B);  // Aumentar probabilidad
            std::cout << "No room generated. New room probability: " << roomProb << std::endl;
        }
    }

    std::cout << "\n=== Enhanced Drunk Agent Finished ===" << std::endl;
    std::cout << "Final position: (" << agentX << ", " << agentY << ")" << std::endl;
    
    return currentMap;
}

// Función para probar diferentes configuraciones del Drunk Agent
void testDrunkAgentConfigurations() {
    std::cout << "\n=== TESTING DIFFERENT DRUNK AGENT CONFIGURATIONS ===" << std::endl;
    
    // Configuración 1: Agente conservador (pocas habitaciones, pocos cambios de dirección)
    {
        std::cout << "\n--- Configuration 1: Conservative Agent ---" << std::endl;
        int W = 20, H = 15;
        Map map1(H, std::vector<int>(W, 0));
        map1 = enhancedDrunkAgent(map1, W, H, 6, 8, 3, 3, 0.1, 0.03, 0.15, 0.02);
        
        int filled = 0;
        for (const auto& row : map1) {
            for (int cell : row) if (cell == 1) filled++;
        }
        std::cout << "Fill percentage: " << (100.0 * filled / (W * H)) << "%" << std::endl;
    }
    
    // Configuración 2: Agente agresivo (muchas habitaciones, muchos cambios)
    {
        std::cout << "\n--- Configuration 2: Aggressive Agent ---" << std::endl;
        int W = 20, H = 15;
        Map map2(H, std::vector<int>(W, 0));
        map2 = enhancedDrunkAgent(map2, W, H, 10, 4, 5, 4, 0.3, 0.15, 0.4, 0.1);
        
        int filled = 0;
        for (const auto& row : map2) {
            for (int cell : row) if (cell == 1) filled++;
        }
        std::cout << "Fill percentage: " << (100.0 * filled / (W * H)) << "%" << std::endl;
    }
    
    // Configuración 3: Agente equilibrado
    {
        std::cout << "\n--- Configuration 3: Balanced Agent ---" << std::endl;
        int W = 20, H = 15;
        Map map3(H, std::vector<int>(W, 0));
        map3 = enhancedDrunkAgent(map3, W, H, 8, 6, 4, 3, 0.2, 0.08, 0.25, 0.05);
        
        int filled = 0;
        for (const auto& row : map3) {
            for (int cell : row) if (cell == 1) filled++;
        }
        std::cout << "Fill percentage: " << (100.0 * filled / (W * H)) << "%" << std::endl;
        printMap(map3);
    }
}

int main() {
    std::cout << "--- CELLULAR AUTOMATA AND DRUNK AGENT SIMULATION ---" << std::endl;

    int mapRows = 15;
    int mapCols = 25;
    Map myMap(mapRows, std::vector<int>(mapCols, 0));

    // Inicializar agente en posición aleatoria (-1, -1 indica primera ejecución)
    int drunkAgentX = -1;
    int drunkAgentY = -1;

    std::cout << "\nInitial map state:" << std::endl;
    printMap(myMap);

    int numIterations = 3;

    // Parámetros de Cellular Automata (sin efecto aún)
    int ca_W = mapCols;
    int ca_H = mapRows;
    int ca_R = 1;
    double ca_U = 0.5;

    // Parámetros del Drunk Agent
    int da_W = mapCols;
    int da_H = mapRows;
    int da_J = 8;        // Número de movimientos
    int da_I = 6;        // Pasos por movimiento
    int da_roomSizeX = 4; // Ancho de habitación
    int da_roomSizeY = 3; // Alto de habitación
    double da_probGenerateRoom = 0.15;    // Probabilidad inicial A
    double da_probIncreaseRoom = 0.08;    // Incremento B
    double da_probChangeDirection = 0.25; // Probabilidad inicial C
    double da_probIncreaseChange = 0.05;  // Incremento D

    for (int iteration = 0; iteration < numIterations; ++iteration) {
        std::cout << "\n========== Iteration " << iteration + 1 << " ==========" << std::endl;

        // Paso del autómata celular (a implementar)
        // myMap = cellularAutomata(myMap, ca_W, ca_H, ca_R, ca_U);

        // Usar la versión mejorada del agente borracho
        myMap = enhancedDrunkAgent(myMap, da_W, da_H, da_J, da_I, da_roomSizeX, da_roomSizeY,
                                  da_probGenerateRoom, da_probIncreaseRoom,
                                  da_probChangeDirection, da_probIncreaseChange);

        std::cout << "\nMap after iteration " << iteration + 1 << ":" << std::endl;
        printMap(myMap);
    }

    std::cout << "\n--- Simulation Finished ---" << std::endl;
    
    // Estadísticas finales
    int totalCells = mapRows * mapCols;
    int filledCells = 0;
    for (const auto& row : myMap) {
        for (int cell : row) {
            if (cell == 1) filledCells++;
        }
    }
    
    std::cout << "Final Statistics:" << std::endl;
    std::cout << "Total cells: " << totalCells << std::endl;
    std::cout << "Filled cells: " << filledCells << std::endl;
    std::cout << "Fill percentage: " << (100.0 * filledCells / totalCells) << "%" << std::endl;
    
    // Probar diferentes configuraciones del agente
    testDrunkAgentConfigurations();
    
    return 0;

    // Probar diferentes configuraciones del Drunk Agent
    testDrunkAgentConfigurations();

    return 0;
}
