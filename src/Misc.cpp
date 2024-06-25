#include <utility>
#include <iostream>
#include <random>
#include <memory>
#include <map>
#include <iomanip>
#include <sstream>
#include "Misc.hpp"

// Función para generar un número aleatorio dentro de un rango
int randomRange(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

std::shared_ptr<RobotDefensor> findActiveDefender(RedBlackTree &rbTree, const std::shared_ptr<Robot> &currentRobot)
{
    std::shared_ptr<RobotDefensor> activeDefender = nullptr;

    // Obtener el equipo del robot actual
    int currentTeam = currentRobot->getTeam();

    // Recorrer el árbol para buscar un RobotDefensor activo con energía del mismo equipo
    for (int j = 1; j <= rbTree.size(); ++j)
    {
        std::shared_ptr<Robot> robot = rbTree.find(j);
        if (robot && robot->getType() == "Defensor")
        {
            std::shared_ptr<RobotDefensor> defensor = std::dynamic_pointer_cast<RobotDefensor>(robot);
            if (defensor && defensor->getEnergyLevel() > 0 && defensor->getTeam() == currentTeam && currentRobot->isInRange(*defensor, 3) && currentRobot != defensor)
            {
                activeDefender = defensor;
                break;
            }
        }
    }

    return activeDefender;
}

int charToInt(char c)
{
    if (c >= '1' && c <= '9')
    {
        return c - '0';
    }
    else if (c == 'A')
    {
        return 10;
    }
    else
    {
        return -1;
    }
}

// Función para imprimir el estado del tablero
void printBattlefield(const RedBlackTree &rbTree, int **rechargeStations, const int numRobots)
{
    char battlefield[FIELD_SIZE][FIELD_SIZE] = {};

    for (int i = 0; i < numRobots / 2; ++i)
    {
        int stationX = rechargeStations[i][0];
        int stationY = rechargeStations[i][1];
        battlefield[stationX][stationY] = 'R';
    }

    int teams[10];
    rbTree.forEach([&](const std::shared_ptr<Robot> &robot)
                   {
                       int x = robot->getX();
                       int y = robot->getY();
                       char symbol = robot->getName()[5];
                       if (robot->getName()[5] == '1' && robot->getName()[6] == '0')
                       {
                           symbol = 'A';
                       }

                       battlefield[x][y] = symbol;
                       teams[charToInt(symbol) - 1] = robot->getTeam(); });

    std::cout << "   ";
    for (int j = 0; j < FIELD_SIZE; ++j)
    {
        std::cout << " " << std::setw(2) << j << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < FIELD_SIZE; ++i)
    {
        std::cout << "   ";
        for (int j = 0; j < FIELD_SIZE; ++j)
        {
            std::cout << "+---";
        }
        std::cout << "+" << std::endl;

        std::cout << " " << std::setw(2) << i << "";
        for (int j = 0; j < FIELD_SIZE; ++j)
        {
            std::cout << "|";
            char symbol = battlefield[i][j];

            if ((symbol >= '1' && symbol <= '9') || symbol == 'A')
            {
                if (teams[charToInt(symbol) - 1] == 1)
                {
                    std::cout << " " << "\033[1;31m" << symbol << "\033[0m" << " "; // Bright Red
                }
                else if (teams[charToInt(symbol) - 1] == 2)
                {
                    std::cout << " " << "\033[1;34m" << symbol << "\033[0m" << " "; // Bright Blue
                }
            }
            else if (symbol == 'R')
            {
                std::cout << " R ";
            }
            else
            {
                std::cout << "   ";
            }
        }
        std::cout << "|" << std::endl;
    }

    std::cout << "   ";
    for (int j = 0; j < FIELD_SIZE; ++j)
    {
        std::cout << "+---";
    }
    std::cout << "+" << std::endl;
}

// Function to mark a position as available in the occupiedPositions array
int markPositionAvailable(int **occupiedPositions, int numRobots, int x, int y)
{
    for (int i = 0; i < numRobots; ++i)
    {
        if (occupiedPositions[i][0] == x && occupiedPositions[i][1] == y)
        {
            // Mark the position as available (for example, using -1 or another flag)
            occupiedPositions[i][0] = -1;
            occupiedPositions[i][1] = -1;
            return i;
        }
    }
    return 0;
}

// Function to count occurrences of a specific position in the occupiedPositions array
int countPosition(int **occupiedPositions, int numRobots, int x, int y)
{
    int count = 0;
    for (int i = 0; i < numRobots; ++i)
    {
        if (occupiedPositions[i][0] == x && occupiedPositions[i][1] == y)
        {
            ++count;
        }
    }
    return count;
}

// Función para simular una batalla entre robots
void simulateBattle(RedBlackTree &rbTree, int numRobots, int **initialPositions, int rounds)
{
    const int numStations = numRobots / 2;

    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    int **rechargeStations = new int *[numStations];
    // Allocate memory for each individual array
    for (int i = 0; i < numStations; ++i)
    {
        rechargeStations[i] = new int[2];
    }

    // Place recharge stations randomly on the battlefield
    for (int i = 0; i < numStations; ++i)
    {
        int stationX = randomRange(MIN_POSITION, MAX_POSITION);
        int stationY = randomRange(MIN_POSITION, MAX_POSITION);
        rechargeStations[i][0] = stationX;
        rechargeStations[i][1] = stationY;
    }

    int **occupiedPositions = new int *[numRobots];
    // Allocate memory for each individual array (assuming each has two elements)
    for (int i = 0; i < numRobots; ++i)
    {
        occupiedPositions[i] = new int[2];
    }

    for (int i = 0; i < numRobots; ++i)
    {
        occupiedPositions[i][0] = initialPositions[i][0];
        occupiedPositions[i][1] = initialPositions[i][1];
    }

    // Generar robots aleatorios y añadirlos al árbol rojo-negro
    int team1Count = 0;
    int team2Count = 0;

    for (int i = 1; i <= numRobots; ++i)
    {
        int robotType = randomRange(1, 3); // Generate random robot type (1, 2, 3)
        int energy = randomRange(50, 100); // Generate random energy (50 to 100)

        int startX = initialPositions[i - 1][0];
        int startY = initialPositions[i - 1][1];

        std::shared_ptr<Robot> robot;
        std::string robotName = "Robot" + std::to_string(i);

        int team;
        if (team1Count <= team2Count)
        {
            team = 1;
            ++team1Count;
        }
        else
        {
            team = 2;
            ++team2Count;
        }

        switch (robotType)
        {
        case 1:
            robot = std::make_shared<RobotAsalto>(i, robotName, energy, startX, startY);
            robot->setTeam(team);
            std::cout << robotName << " is of type Asalto, Team " << team << std::endl;
            break;
        case 2:
            robot = std::make_shared<RobotExplorador>(i, robotName, energy, startX, startY);
            robot->setTeam(team);
            std::cout << robotName << " is of type Explorador, Team " << team << std::endl;
            break;
        case 3:
            robot = std::make_shared<RobotDefensor>(i, robotName, energy, startX, startY);
            robot->setTeam(team);
            std::cout << robotName << " is of type Defensor, Team " << team << std::endl;
            break;
        }

        rbTree.insert(robot);
    }

    // Simulación de la batalla
    for (int i = 0; i < rounds; i++)
    {
        for (int i = 0; i < numRobots; ++i)
        {
            int last_deleted = 0;
            std::cout << "\nEstado del tablero en el turno del Robot" << i + 1 << ":\n";
            printBattlefield(rbTree, rechargeStations, numRobots);

            std::shared_ptr<Robot> currentRobot = rbTree.find(i + 1); // Obtener el robot por su ID
            if (currentRobot)
            {
                if (currentRobot->getEnergyLevel() <= 0)
                {
                    last_deleted = markPositionAvailable(occupiedPositions, numRobots, currentRobot->getX(), currentRobot->getY());
                    rbTree.remove(currentRobot->getID());
                    numRobots--;
                    std::cout << currentRobot->getName() << " ha sido eliminado debido a energía 0 o menor.\n";
                    continue;
                }

                bool stationRecharge = false;

                // Verificar si el robot está en una estación de recarga
                for (int i = 0; i < numStations; ++i)
                {
                    if (currentRobot->getX() == occupiedPositions[i][0] && currentRobot->getY() == occupiedPositions[i][1])
                    {
                        currentRobot->recharge(RECHARGE_ENERGY);
                        std::cout << "Robot recharged at (" << occupiedPositions[i][0] << ", " << occupiedPositions[i][1] << ")\n";
                        break;
                    }
                }

                if (stationRecharge)
                    continue;

                std::map<std::string, int> robot_map = {
                    {"Asalto", 1},
                    {"Explorador", 2},
                    {"Defensor", 3}};
                // Determinar el tipo de robot y realizar la acción correspondiente
                switch (robot_map.find(currentRobot->getType())->second)
                {
                case 1:
                { // RobotAsalto
                    int actionType;
                    std::cout << "Seleccione la accion a realizar con el robot asalto: \n";
                    std::cout << "1: Atacar \n";
                    std::cout << "2: Mover \n";
                    std::cin >> actionType;
                    if (actionType == 1)
                    {
                        // Buscar enemigos dentro de un radio de 3m
                        std::cout << currentRobot->getName() << " searching for enemies within a 3m radius" << "\n";
                        bool enemyFound = false;

                        // Mostrar lista de robots enemigos dentro del radio
                        std::cout << "Lista de Robots Enemigos dentro del radio:\n";
                        for (int j = 1; j <= numRobots; ++j)
                        {
                            if (i + 1 == j)
                                continue;
                            std::shared_ptr<Robot> targetRobot = rbTree.find(j);
                            if (targetRobot && currentRobot->isInRange(*targetRobot, 3) && currentRobot != targetRobot)
                            {
                                if (currentRobot->getTeam() != targetRobot->getTeam())
                                {
                                    std::cout << targetRobot->getName() << " (ID: " << targetRobot->getID() << ") ";
                                    enemyFound = true;
                                }
                            }
                        }
                        std::cout << "\n";

                        if (enemyFound)
                        {
                            // Elegir a qué robot atacar
                            int targetID;
                            std::cout << "Seleccione el ID del robot que desea atacar: ";
                            std::cin >> targetID;

                            std::shared_ptr<Robot> targetRobot = rbTree.find(targetID);
                            if (targetRobot && currentRobot->isInRange(*targetRobot, 3) && currentRobot->getTeam() != targetRobot->getTeam())
                            {
                                std::shared_ptr<RobotDefensor> defensor = findActiveDefender(rbTree, targetRobot);
                                if (defensor)
                                {
                                    std::cout << currentRobot->getName() << " attacks " << targetRobot->getName() << std::endl;
                                    defensor->defender(*targetRobot);
                                }
                                else
                                {
                                    currentRobot->attack(*targetRobot);
                                    if (targetRobot->getEnergyLevel() <= 0)
                                    {
                                        last_deleted = markPositionAvailable(occupiedPositions, numRobots, targetRobot->getX(), targetRobot->getY());
                                        rbTree.remove(targetRobot->getID());
                                        numRobots--;
                                        std::cout << targetRobot->getName() << " ha sido eliminado debido a energía 0 o menor.\n";
                                    }
                                }
                            }
                            else
                            {
                                std::cout << "El robot seleccionado no es un enemigo válido o está fuera de alcance.\n";
                            }
                        }
                        else
                        {
                            std::cout << "No se encontraron enemigos dentro del radio.\n";

                            // Mover a las coordenadas especificadas por el usuario
                            int newX, newY;
                            std::cout << "Ingrese las nuevas coordenadas X y Y para mover el robot:\n";
                            std::cout << "X: ";
                            std::cin >> newX;
                            std::cout << "Y: ";
                            std::cin >> newY;

                            // Ajustar las coordenadas dentro de los límites del campo de batalla
                            if (newX < MIN_POSITION)
                                newX = MIN_POSITION;
                            if (newX > MAX_POSITION)
                                newX = MAX_POSITION;
                            if (newY < MIN_POSITION)
                                newY = MIN_POSITION;
                            if (newY > MAX_POSITION)
                                newY = MAX_POSITION;

                            // Verificar si las nuevas coordenadas están ocupadas
                            if (!countPosition(occupiedPositions, numRobots, newX, newY))
                            {
                                last_deleted = markPositionAvailable(occupiedPositions, numRobots, currentRobot->getX(), currentRobot->getY());
                                currentRobot->move(newX, newY);
                                occupiedPositions[last_deleted][0] = newX;
                                occupiedPositions[last_deleted][1] = newY;
                            }
                            else
                            {
                                std::cout << "Las coordenadas especificadas están ocupadas. El robot no se ha movido.\n";
                            }
                        }
                    }
                    else if (actionType == 2)
                    {
                        // Mover a las coordenadas especificadas por el usuario dentro de un radio de 3 casillas
                        int newX, newY;
                        std::cout << "Ingrese las nuevas coordenadas X y Y para mover el robot (dentro de un radio de 3 casillas):\n";
                        std::cout << "X: ";
                        std::cin >> newX;
                        std::cout << "Y: ";
                        std::cin >> newY;

                        // Calcular la distancia en X y Y desde la posición actual del robot
                        int deltaX = newX - currentRobot->getX();
                        int deltaY = newY - currentRobot->getY();

                        // Limitar el movimiento a un radio de 3 casillas
                        if (std::abs(deltaX) > 3)
                        {
                            if (deltaX > 0)
                                newX = currentRobot->getX() + 3;
                            else
                                newX = currentRobot->getX() - 3;
                        }

                        if (std::abs(deltaY) > 3)
                        {
                            if (deltaY > 0)
                                newY = currentRobot->getY() + 3;
                            else
                                newY = currentRobot->getY() - 3;
                        }

                        // Ajustar las coordenadas dentro de los límites del campo de batalla
                        if (newX < MIN_POSITION)
                            newX = MIN_POSITION;
                        if (newX > MAX_POSITION)
                            newX = MAX_POSITION;
                        if (newY < MIN_POSITION)
                            newY = MIN_POSITION;
                        if (newY > MAX_POSITION)
                            newY = MAX_POSITION;

                        // Verificar si las nuevas coordenadas están ocupadas
                        if (!countPosition(occupiedPositions, numRobots, newX, newY))
                        {
                            last_deleted = markPositionAvailable(occupiedPositions, numRobots, currentRobot->getX(), currentRobot->getY());
                            currentRobot->move(newX, newY);
                            occupiedPositions[last_deleted][0] = newX;
                            occupiedPositions[last_deleted][1] = newY;
                        }
                        else
                        {
                            std::cout << "Las coordenadas especificadas están ocupadas. El robot no se ha movido.\n";
                        }
                    }
                    else
                    {
                        std::cout << "Acción no válida para el Robot Asalto.\n";
                    }
                    break;
                }
                case 2:
                { // RobotExplorador
                    int actionType;
                    std::cout << "Seleccione la accion a realizar con el robot explorador: \n";
                    std::cout << "1: Explorar \n";
                    std::cout << "2: Atacar \n";
                    std::cin >> actionType;
                    if (actionType == 1)
                    {
                        // Acción: Explorar
                        int newX, newY;
                        std::cout << "Ingrese las nuevas coordenadas X y Y para mover el robot (dentro de un radio de 5 casillas):\n";
                        std::cout << "X: ";
                        std::cin >> newX;
                        std::cout << "Y: ";
                        std::cin >> newY;

                        // Calcular la distancia en X y Y desde la posición actual del robot
                        int deltaX = newX - currentRobot->getX();
                        int deltaY = newY - currentRobot->getY();

                        // Limitar el movimiento a un radio de 3 casillas
                        if (std::abs(deltaX) > 5)
                        {
                            if (deltaX > 0)
                                newX = currentRobot->getX() + 5;
                            else
                                newX = currentRobot->getX() - 5;
                        }

                        if (std::abs(deltaY) > 5)
                        {
                            if (deltaY > 0)
                                newY = currentRobot->getY() + 5;
                            else
                                newY = currentRobot->getY() - 5;
                        }

                        // Ajustar las coordenadas dentro de los límites del campo de batalla
                        if (newX < MIN_POSITION)
                            newX = MIN_POSITION;
                        if (newX > MAX_POSITION)
                            newX = MAX_POSITION;
                        if (newY < MIN_POSITION)
                            newY = MIN_POSITION;
                        if (newY > MAX_POSITION)
                            newY = MAX_POSITION;

                        // Verificar si las nuevas coordenadas están ocupadas
                        if (!countPosition(occupiedPositions, numRobots, newX, newY))
                        {
                            markPositionAvailable(occupiedPositions, numRobots, currentRobot->getX(), currentRobot->getY());
                            currentRobot->move(newX, newY);
                            occupiedPositions[last_deleted][0] = newX;
                            occupiedPositions[last_deleted][1] = newY;
                        }
                        else
                        {
                            std::cout << "Las coordenadas especificadas están ocupadas. El robot no se ha movido.\n";
                        }
                    }
                    else if (actionType == 2)
                    {
                        // Acción: Buscar y atacar enemigos dentro de un radio de 5m
                        std::cout << currentRobot->getName() << " searching for enemies within a 5m radius" << "\n";
                        std::shared_ptr<Robot> targetRobot;
                        int targetID;
                        bool enemyFound = false;

                        // Mostrar lista de robots enemigos dentro del radio
                        std::cout << "Lista de Robots Enemigos dentro del radio:\n";
                        for (int j = 1; j <= numRobots; ++j)
                        {
                            if (i + 1 == j)
                                continue;
                            std::shared_ptr<Robot> targetRobot = rbTree.find(j);
                            if (targetRobot && currentRobot->isInRange(*targetRobot, 5) && currentRobot != targetRobot)
                            {
                                if (currentRobot->getTeam() != targetRobot->getTeam())
                                {
                                    std::cout << targetRobot->getName() << " (ID: " << targetRobot->getID() << ") ";
                                    enemyFound = true;
                                }
                            }
                        }
                        std::cout << "\n";

                        if (enemyFound)
                        {
                            // Elegir a qué robot atacar
                            std::cout << "Seleccione el ID del robot que desea atacar: ";
                            std::cin >> targetID;

                            std::shared_ptr<Robot> targetRobot = rbTree.find(targetID);
                            if (targetRobot && currentRobot->isInRange(*targetRobot, 5) && currentRobot->getTeam() != targetRobot->getTeam())
                            {
                                std::shared_ptr<RobotDefensor> defensor = findActiveDefender(rbTree, targetRobot);
                                if (defensor)
                                {
                                    std::cout << currentRobot->getName() << " attacks " << targetRobot->getName() << std::endl;
                                    defensor->defender(*targetRobot);
                                }
                                else
                                {
                                    currentRobot->attack(*targetRobot);
                                    if (targetRobot->getEnergyLevel() <= 0)
                                    {
                                        markPositionAvailable(occupiedPositions, numRobots, targetRobot->getX(), targetRobot->getY());
                                        rbTree.remove(targetRobot->getID());
                                        numRobots--;
                                        std::cout << targetRobot->getName() << " ha sido eliminado debido a energía 0 o menor.\n";
                                    }
                                }
                            }
                            else
                            {
                                std::cout << "El robot seleccionado no es un enemigo válido o está fuera de alcance.\n";
                            }
                        }
                        else
                        {
                            std::cout << "No se encontraron enemigos dentro del radio.\n";
                        }
                    }
                    else
                    {
                        std::cout << "Acción no válida para el Robot Explorador.\n";
                    }
                    break;
                }
                case 3:
                { // RobotDefensor
                    int userChoice;
                    std::cout << "Seleccione la accion a realizar con el robot defensor: \n";
                    std::cout << "1. Atacar\n";
                    std::cout << "2. Mover\n";
                    std::cout << "3. Transferir Energía\n";
                    std::cout << "Ingrese su elección: ";
                    std::cin >> userChoice;

                    if (userChoice == 1)
                    {
                        std::cout << currentRobot->getName() << " searching for enemies within a 3m radius" << "\n";
                        int enemiesInRange[10];
                        int numEnemiesInRange = 0;

                        // Buscar todos los enemigos dentro de un radio de 3 metros
                        for (int targetID = 1; targetID <= numRobots; ++targetID)
                        {
                            std::shared_ptr<Robot> targetRobot = rbTree.find(targetID);
                            if (targetRobot && targetRobot->getTeam() != currentRobot->getTeam() && currentRobot->isInRange(*targetRobot, 3) && targetRobot != currentRobot)
                            {
                                if (numEnemiesInRange < 10)
                                {
                                    enemiesInRange[numEnemiesInRange] = targetID;
                                    ++numEnemiesInRange;
                                    std::cout << "ID: " << targetID << ", Name: " << targetRobot->getName() << "\n";
                                }
                                else
                                {
                                    std::cout << "Maximum number of enemies in range reached.\n";
                                    break; // Exit loop if maximum capacity reached
                                }
                            }
                        }

                        if (numEnemiesInRange > 0)
                        {
                            int chosenID;
                            std::cout << "Ingrese el ID del robot enemigo a atacar: ";
                            std::cin >> chosenID;
                            bool validChoice = false;
                            for (int i = 0; i < numEnemiesInRange; ++i)
                            {
                                if (enemiesInRange[i] == chosenID)
                                {
                                    validChoice = true;
                                    break;
                                }
                            }

                            // Verificar si el ID ingresado está dentro de los enemigos encontrados
                            if (validChoice)
                            {
                                std::shared_ptr<Robot> targetRobot = rbTree.find(chosenID);
                                std::shared_ptr<RobotDefensor> defensor = findActiveDefender(rbTree, targetRobot);
                                if (defensor)
                                {
                                    std::cout << currentRobot->getName() << " attacks " << targetRobot->getName() << std::endl;
                                    defensor->defender(*targetRobot);
                                }
                                else
                                {
                                    currentRobot->attack(*targetRobot);
                                    if (targetRobot->getEnergyLevel() <= 0)
                                    {
                                        markPositionAvailable(occupiedPositions, numRobots, targetRobot->getX(), targetRobot->getY());
                                        rbTree.remove(targetRobot->getID());
                                        numRobots--;
                                        std::cout << targetRobot->getName() << " ha sido eliminado debido a energía 0 o menor.\n";
                                    }
                                }
                            }
                            else
                            {
                                std::cout << "El ID ingresado no corresponde a ningún enemigo en el rango de 3 metros.\n";
                            }
                        }
                        else
                        {
                            std::cout << "No se encontraron enemigos dentro del radio de 3m.\n";
                        }
                    }
                    else if (userChoice == 2)
                    {
                        int newX, newY;
                        bool newPositionFound = false;

                        std::cout << "Ingrese las nuevas coordenadas X y Y para mover el robot dentro de un radio de 3 metros:\n";

                        do
                        {
                            std::cout << "X: ";
                            std::cin >> newX;
                            std::cout << "Y: ";
                            std::cin >> newY;

                            int deltaX = std::abs(newX - currentRobot->getX());
                            int deltaY = std::abs(newY - currentRobot->getY());

                            // Verificar que las nuevas coordenadas estén dentro de un radio de 3 metros
                            if (deltaX <= 3 && deltaY <= 3)
                            {
                                // Ajustar las coordenadas dentro de los límites del campo de batalla
                                if (newX < MIN_POSITION)
                                    newX = MIN_POSITION;
                                if (newX > MAX_POSITION)
                                    newX = MAX_POSITION;
                                if (newY < MIN_POSITION)
                                    newY = MIN_POSITION;
                                if (newY > MAX_POSITION)
                                    newY = MAX_POSITION;

                                // Verificar si las nuevas coordenadas no están ocupadas
                                if (!countPosition(occupiedPositions, numRobots, newX, newY))
                                {
                                    newPositionFound = true;
                                }
                                else
                                {
                                    std::cout << "Las coordenadas especificadas están ocupadas. Por favor, ingrese otras coordenadas.\n";
                                }
                            }
                            else
                            {
                                std::cout << "Las coordenadas especificadas están fuera del radio de 3 metros. Por favor, ingrese otras coordenadas.\n";
                            }
                        } while (!newPositionFound);

                        last_deleted = markPositionAvailable(occupiedPositions, numRobots, currentRobot->getX(), currentRobot->getY());
                        currentRobot->move(newX, newY);
                        occupiedPositions[last_deleted][0] = newX;
                        occupiedPositions[last_deleted][1] = newY;
                    }
                    else if (userChoice == 3)
                    {
                        int alliesInRange[10];
                        int numAlliesInRange = 0;

                        std::cout << currentRobot->getName() << " searching for allies within a 3m radius" << "\n";

                        // Buscar aliados dentro de un radio de 3 metros
                        for (int targetID = 1; targetID <= numRobots; ++targetID)
                        {
                            std::shared_ptr<Robot> targetRobot = rbTree.find(targetID);
                            if (targetRobot && targetRobot->getTeam() == currentRobot->getTeam() && currentRobot->isInRange(*targetRobot, 3) && targetRobot != currentRobot)
                            {
                                if (numAlliesInRange < 10)
                                {
                                    alliesInRange[numAlliesInRange] = targetID;
                                    ++numAlliesInRange;
                                }
                                else
                                {
                                    std::cout << "Se alcanzó el número máximo de aliados en rango.\n";
                                    break;
                                }
                            }
                        }

                        if (numAlliesInRange > 0)
                        {
                            std::cout << "Aliados en un radio de 3m:\n";
                            for (int id : alliesInRange)
                            {
                                if (id == 0)
                                    continue;
                                std::shared_ptr<Robot> ally = rbTree.find(id);
                                std::cout << "ID: " << id << ", Nombre: " << ally->getName() << "\n";
                            }

                            int chosenID;
                            std::cout << "Ingrese el ID del aliado al que desea transferir energía: ";
                            std::cin >> chosenID;

                            bool validChoice = false;
                            for (int i = 0; i < numAlliesInRange; ++i)
                            {
                                if (alliesInRange[i] == chosenID)
                                {
                                    validChoice = true;
                                    break;
                                }
                            }

                            // Verificar si el ID ingresado es válido
                            if (validChoice)
                            {
                                std::shared_ptr<Robot> targetRobot = rbTree.find(chosenID);
                                currentRobot->transferEnergy(*targetRobot);
                                std::cout << "Energía transferida a " << targetRobot->getName() << ".\n";
                            }
                            else
                            {
                                std::cout << "ID no válido. No se realizó la transferencia de energía.\n";
                            }
                        }
                        else
                        {
                            std::cout << "No se encontraron aliados dentro del radio de 3m.\n";
                        }
                    }
                    else
                    {
                        std::cout << "Selección no válida. Saliendo de la acción del RobotDefensor.\n";
                    }
                    break;
                }
                }
            }
        }
    }

    // Free the memory
    for (int i = 0; i < numStations; ++i)
    {
        delete[] rechargeStations[i]; // Delete each sub-array
    }
    delete[] rechargeStations; // Delete the main array

    // Free the memory
    for (int i = 0; i < numRobots; ++i)
    {
        delete[] occupiedPositions[i]; // Delete each sub-array
    }
    delete[] occupiedPositions; // Delete the main array
    std::cout << "\nSimulación finalizada.\n";
}

// Función para simular una batalla entre robots
void simulateBattle(RedBlackTree &rbTree, int numRobots, int rounds)
{

    const int numStations = numRobots / 2;

    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    int **rechargeStations = new int *[numStations];

    // Allocate memory for each individual array
    for (int i = 0; i < numStations; ++i)
    {
        rechargeStations[i] = new int[2]; // Allocating memory for X and Y coordinates
    }

    // Place recharge stations randomly on the battlefield
    for (int i = 0; i < numStations; ++i)
    {
        int stationX = randomRange(MIN_POSITION, MAX_POSITION);
        int stationY = randomRange(MIN_POSITION, MAX_POSITION);
        rechargeStations[i][0] = stationX;
        rechargeStations[i][1] = stationY;
    }

    // Generar robots aleatorios y añadirlos al árbol rojo-negro
    int **occupiedPositions = new int *[numRobots];
    int team1Count = 0;
    int team2Count = 0;

    // Allocate memory for each individual array (assuming each has two elements)
    for (int i = 0; i < numRobots; ++i)
    {
        occupiedPositions[i] = new int[2];
    }

    for (int i = 1; i <= numRobots; ++i)
    {
        int robotType = randomRange(1, 3); // Generate random robot type (1, 2, 3)
        int energy = randomRange(50, 100); // Generate random energy (50 to 100)

        int startX, startY;
        do
        {
            startX = randomRange(MIN_POSITION, MAX_POSITION); // Initial position in X
            startY = randomRange(MIN_POSITION, MAX_POSITION); // Initial position in Y
        } while (countPosition(occupiedPositions, numRobots, startX, startY) > 0); // Ensure the position is unique

        occupiedPositions[i - 1][0] = startX;
        occupiedPositions[i - 1][1] = startY;

        std::shared_ptr<Robot> robot;
        std::string robotName = "Robot" + std::to_string(i);

        int team;
        if (team1Count <= team2Count)
        {
            team = 1;
            ++team1Count;
        }
        else
        {
            team = 2;
            ++team2Count;
        }

        switch (robotType)
        {
        case 1:
            robot = std::make_shared<RobotAsalto>(i, robotName, energy, startX, startY);
            robot->setTeam(team);
            std::cout << robotName << " is of type Asalto, Team " << team << std::endl;
            break;
        case 2:
            robot = std::make_shared<RobotExplorador>(i, robotName, energy, startX, startY);
            robot->setTeam(team);
            std::cout << robotName << " is of type Explorador, Team " << team << std::endl;
            break;
        case 3:
            robot = std::make_shared<RobotDefensor>(i, robotName, energy, startX, startY);
            robot->setTeam(team);
            std::cout << robotName << " is of type Defensor, Team " << team << std::endl;
            break;
        }

        rbTree.insert(robot);
    }

    // Simulación de la batalla
    for (int i = 0; i < rounds; i++)
    {
        for (int i = 0; i < numRobots; ++i)
        {
            std::cout << "\nEstado del tablero en el turno del Robot" << i + 1 << ":\n";
            printBattlefield(rbTree, rechargeStations, numRobots);

            std::shared_ptr<Robot> currentRobot = rbTree.find(i + 1); // Obtener el robot por su ID
            if (currentRobot)
            {
                if (currentRobot->getEnergyLevel() <= 0)
                {
                    markPositionAvailable(occupiedPositions, numRobots, currentRobot->getX(), currentRobot->getY());
                    rbTree.remove(currentRobot->getID());
                    numRobots--;
                    std::cout << currentRobot->getName() << " ha sido eliminado debido a energía 0 o menor.\n";
                    continue;
                }

                int actionType = randomRange(1, 100);
                bool stationRecharge = false;

                // Verificar si el robot está en una estación de recarga
                for (int i = 0; i < numStations; ++i)
                {
                    if (currentRobot->getX() == occupiedPositions[i][0] && currentRobot->getY() == occupiedPositions[i][1])
                    {
                        currentRobot->recharge(RECHARGE_ENERGY);
                        std::cout << "Robot recharged at (" << occupiedPositions[i][0] << ", " << occupiedPositions[i][1] << ")\n";
                        break;
                    }
                }

                if (stationRecharge)
                    continue;

                std::map<std::string, int> robot_map = {
                    {"Asalto", 1},
                    {"Explorador", 2},
                    {"Defensor", 3}};

                switch (robot_map.find(currentRobot->getType())->second)
                {
                case 1:
                { // RobotAsalto
                    std::cout << currentRobot->getName() << " searching for enemies within a 3m radius" << "\n";
                    bool enemyFound = false;
                    for (int j = 1; j <= numRobots; ++j)
                    {
                        if (i + 1 == j)
                            continue;
                        std::shared_ptr<Robot> targetRobot = rbTree.find(j);
                        if (targetRobot && currentRobot->isInRange(*targetRobot, 3) && currentRobot != targetRobot)
                        {
                            if (currentRobot->getTeam() != targetRobot->getTeam())
                            {
                                enemyFound = true;
                                if (actionType <= 70)
                                {
                                    // Verificar si hay un RobotDefensor activo con energía para defender
                                    std::shared_ptr<RobotDefensor> defensor = findActiveDefender(rbTree, targetRobot);
                                    if (defensor)
                                    {
                                        std::cout << currentRobot->getName() << " attacks " << targetRobot->getName() << std::endl;
                                        defensor->defender(*targetRobot);
                                    }
                                    else
                                    {
                                        currentRobot->attack(*targetRobot);
                                        if (targetRobot->getEnergyLevel() <= 0)
                                        {
                                            markPositionAvailable(occupiedPositions, numRobots, targetRobot->getX(), targetRobot->getY());
                                            rbTree.remove(targetRobot->getID());
                                            numRobots--;
                                            std::cout << targetRobot->getName() << " ha sido eliminado debido a energía 0 o menor.\n";
                                            continue;
                                        }
                                    }
                                }
                                else
                                {

                                    std::cout << currentRobot->getName() << " decided not to attack " << targetRobot->getName() << "\n";
                                }
                                break;
                            }
                        }
                    }
                    if (!enemyFound || actionType > 70)
                    {
                        // Buscar el enemigo más cercano
                        std::shared_ptr<Robot> nearestEnemy = nullptr;
                        int minDistance = std::numeric_limits<int>::max();

                        for (int j = 1; j <= numRobots; ++j)
                        {
                            if (i + 1 == j)
                                continue;
                            std::shared_ptr<Robot> targetRobot = rbTree.find(j);
                            if (targetRobot && currentRobot->getTeam() != targetRobot->getTeam())
                            {
                                int distance = std::abs(currentRobot->getX() - targetRobot->getX()) + std::abs(currentRobot->getY() - targetRobot->getY());
                                if (distance < minDistance)
                                {
                                    minDistance = distance;
                                    nearestEnemy = targetRobot;
                                }
                            }
                        }

                        if (nearestEnemy)
                        {
                            int deltaX = nearestEnemy->getX() - currentRobot->getX();
                            int deltaY = nearestEnemy->getY() - currentRobot->getY();

                            // Limitar el movimiento a un máximo de 3 casillas
                            if (deltaX > 3)
                                deltaX = 3;
                            if (deltaX < -3)
                                deltaX = -3;
                            if (deltaY > 3)
                                deltaY = 3;
                            if (deltaY < -3)
                                deltaY = -3;

                            int newX = currentRobot->getX() + deltaX;
                            int newY = currentRobot->getY() + deltaY;

                            // Ajustar las coordenadas dentro de los límites del campo de batalla
                            if (newX < MIN_POSITION)
                                newX = MIN_POSITION;
                            if (newX > MAX_POSITION)
                                newX = MAX_POSITION;
                            if (newY < MIN_POSITION)
                                newY = MIN_POSITION;
                            if (newY > MAX_POSITION)
                                newY = MAX_POSITION;

                            bool newPositionFound = false;
                            if (!countPosition(occupiedPositions, numRobots, newX, newY))
                            {
                                newPositionFound = true;
                            }
                            else
                            {
                                // Si la posición está ocupada, buscar una posición cercana válida
                                for (int dx = -1; dx <= 1; ++dx)
                                {
                                    for (int dy = -1; dy <= 1; ++dy)
                                    {
                                        int tryX = newX + dx;
                                        int tryY = newY + dy;
                                        if (tryX >= MIN_POSITION && tryX <= MAX_POSITION && tryY >= MIN_POSITION && tryY <= MAX_POSITION && !countPosition(occupiedPositions, numRobots, newX, newY))
                                        {
                                            newX = tryX;
                                            newY = tryY;
                                            newPositionFound = true;
                                            break;
                                        }
                                    }
                                    if (newPositionFound)
                                        break;
                                }
                            }

                            int last_deleted = 0;
                            if (newPositionFound)
                            {
                                last_deleted = markPositionAvailable(occupiedPositions, numRobots, currentRobot->getX(), currentRobot->getY());
                                currentRobot->move(newX, newY);
                                occupiedPositions[last_deleted][0] = newX;
                                occupiedPositions[last_deleted][1] = newY;
                            }
                        }
                    }
                    break;
                }
                case 2:
                { // RobotExplorador
                    if (actionType <= 60)
                    {
                        int deltaX, deltaY, newX, newY;
                        bool newPositionFound = false;

                        do
                        {
                            deltaX = randomRange(-5, 5);
                            deltaY = randomRange(-5, 5);
                            newX = currentRobot->getX() + deltaX;
                            newY = currentRobot->getY() + deltaY;

                            // Ajustar las coordenadas dentro de los límites del campo de batalla
                            if (newX < MIN_POSITION)
                                newX = MIN_POSITION;
                            if (newX > MAX_POSITION)
                                newX = MAX_POSITION;
                            if (newY < MIN_POSITION)
                                newY = MIN_POSITION;
                            if (newY > MAX_POSITION)
                                newY = MAX_POSITION;

                            // Verificar si las nuevas coordenadas no están ocupadas
                            if (!countPosition(occupiedPositions, numRobots, newX, newY))
                            {
                                newPositionFound = true;
                            }
                        } while (!newPositionFound);

                        int last_deleted = markPositionAvailable(occupiedPositions, numRobots, currentRobot->getX(), currentRobot->getY());
                        currentRobot->move(newX, newY);
                        occupiedPositions[last_deleted][0] = newX;
                        occupiedPositions[last_deleted][1] = newY;
                    }
                    else
                    {
                        std::cout << currentRobot->getName() << " searching for enemies within a 5m radius" << "\n";
                        std::shared_ptr<Robot> targetRobot;
                        int targetID = 1;
                        do
                        {
                            targetRobot = rbTree.find(targetID);
                            targetID++;
                        } while ((!targetRobot || targetRobot->getTeam() == currentRobot->getTeam() ||
                                  !currentRobot->isInRange(*targetRobot, 5) || targetRobot == currentRobot) &&
                                 targetID < numRobots);
                        if (targetRobot && targetRobot->getTeam() != currentRobot->getTeam() && currentRobot->isInRange(*targetRobot, 5) && targetRobot != currentRobot)
                        {
                            std::shared_ptr<RobotDefensor> defensor = findActiveDefender(rbTree, targetRobot);
                            if (defensor)
                            {
                                std::cout << currentRobot->getName() << " attacks " << targetRobot->getName() << std::endl;
                                defensor->defender(*targetRobot);
                            }
                            else
                            {
                                currentRobot->attack(*targetRobot);
                                if (targetRobot->getEnergyLevel() <= 0)
                                {
                                    markPositionAvailable(occupiedPositions, numRobots, targetRobot->getX(), targetRobot->getY());
                                    rbTree.remove(targetRobot->getID());
                                    numRobots--;
                                    std::cout << targetRobot->getName() << " ha sido eliminado debido a energía 0 o menor.\n";
                                    continue;
                                }
                            }
                        }
                    }
                    break;
                }
                case 3:
                { // RobotDefensor
                    if (actionType <= 33)
                    {
                        std::cout << currentRobot->getName() << " searching for enemies within a 3m radius" << "\n";
                        std::shared_ptr<Robot> targetRobot;
                        int targetID = 1;
                        do
                        {
                            targetRobot = rbTree.find(targetID);
                            targetID++;
                        } while ((!targetRobot || targetRobot->getTeam() == currentRobot->getTeam() || !currentRobot->isInRange(*targetRobot, 3) || targetRobot == currentRobot) && targetID < numRobots);
                        if (targetRobot && targetRobot->getTeam() != currentRobot->getTeam() && currentRobot->isInRange(*targetRobot, 3) && targetRobot != currentRobot)
                        {
                            std::shared_ptr<RobotDefensor> defensor = findActiveDefender(rbTree, targetRobot);
                            if (defensor)
                            {
                                std::cout << currentRobot->getName() << " attacks " << targetRobot->getName() << std::endl;
                                defensor->defender(*targetRobot);
                            }
                            else
                            {
                                currentRobot->attack(*targetRobot);
                                if (targetRobot->getEnergyLevel() <= 0)
                                {
                                    markPositionAvailable(occupiedPositions, numRobots, targetRobot->getX(), targetRobot->getY());
                                    rbTree.remove(targetRobot->getID());
                                    numRobots--;
                                    std::cout << targetRobot->getName() << " ha sido eliminado debido a energía 0 o menor.\n";
                                    continue;
                                }
                            }
                        }
                    }
                    else if (actionType <= 66)
                    {
                        int deltaX, deltaY, newX, newY;
                        bool newPositionFound = false;

                        do
                        {
                            deltaX = randomRange(-3, 3);
                            deltaY = randomRange(-3, 3);
                            newX = currentRobot->getX() + deltaX;
                            newY = currentRobot->getY() + deltaY;

                            // Ajustar las coordenadas dentro de los límites del campo de batalla
                            if (newX < MIN_POSITION)
                                newX = MIN_POSITION;
                            if (newX > MAX_POSITION)
                                newX = MAX_POSITION;
                            if (newY < MIN_POSITION)
                                newY = MIN_POSITION;
                            if (newY > MAX_POSITION)
                                newY = MAX_POSITION;

                            // Verificar si las nuevas coordenadas no están ocupadas
                            if (!countPosition(occupiedPositions, numRobots, newX, newY))
                            {
                                newPositionFound = true;
                            }
                        } while (!newPositionFound);

                        int last_deleted = markPositionAvailable(occupiedPositions, numRobots, currentRobot->getX(), currentRobot->getY());
                        currentRobot->move(newX, newY);
                        occupiedPositions[last_deleted][0] = newX;
                        occupiedPositions[last_deleted][1] = newY;
                    }
                    else
                    {
                        std::cout << currentRobot->getName() << " searching for allies within a 3m radius" << "\n";
                        std::shared_ptr<Robot> targetRobot;
                        int targetID = 1;
                        do
                        {
                            targetRobot = rbTree.find(targetID);
                            targetID++;
                        } while ((!targetRobot || targetRobot->getTeam() != currentRobot->getTeam() || !currentRobot->isInRange(*targetRobot, 3) || targetRobot == currentRobot) && targetID < numRobots);
                        if (targetRobot && targetRobot->getTeam() == currentRobot->getTeam() && currentRobot->isInRange(*targetRobot, 3) && targetRobot != currentRobot)
                        {
                            currentRobot->transferEnergy(*targetRobot);
                        }
                    }
                    break;
                }
                }
            }
        }
    }

    // Free the memory
    for (int i = 0; i < numRobots * rounds; ++i)
    {
        delete[] occupiedPositions[i]; // Delete each sub-array
    }
    delete[] occupiedPositions; // Delete the main array

    // Free the memory
    for (int i = 0; i < numStations; ++i)
    {
        delete[] rechargeStations[i]; // Delete each sub-array
    }
    delete[] rechargeStations; // Delete the main array
}
