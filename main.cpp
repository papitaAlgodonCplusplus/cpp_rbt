
#include "Misc.hpp"

int main()
{
  int gameMode;
  int rounds;
  std::cout << "Seleccione el modo de juego:\n"
            << "1. Modo Manual\n"
            << "2. Modo Simulación\n"
            << "Opción: ";
  std::cin >> gameMode;

  if (gameMode == 1)
  {
    // Modo Manual
    int numRobots;
    std::cout << "Ingrese el número de robots en la simulación (máximo 10): ";
    std::cin >> numRobots;

    if (numRobots < 1 || numRobots > MAX_ROBOTS)
    {
      std::cout << "Número inválido de robots. Terminando el programa.\n";
      return 1;
    }

    std::cout << "Ingrese el número de rounds (máximo 10): ";
    std::cin >> rounds;

    if (rounds < 1 || rounds > MAX_ROUNDS)
    {
      std::cout << "Número inválido de rounds. Terminando el programa.\n";
      return 1;
    }

    int **initialPositions = new int *[COLS];
    // Allocate memory for each individual array
    for (int i = 0; i < COLS; ++i)
    {
      initialPositions[i] = new int[10];
    }

    for (int i = 0; i < numRobots; ++i)
    {
      int posX, posY;
      std::cout << "Ingrese la posición inicial X para el Robot " << i + 1 << " (entre 0 y 9): ";
      std::cin >> posX;
      std::cout << "Ingrese la posición inicial Y para el Robot " << i + 1 << " (entre 0 y 9): ";
      std::cin >> posY;

      if (posX < MIN_POSITION || posX > MAX_POSITION || posY < MIN_POSITION || posY > MAX_POSITION)
      {
        std::cout << "Posición fuera de los límites. Terminando el programa.\n";
        return 1;
      }

      initialPositions[i][0] = posX;
      initialPositions[i][1] = posY;
    }

    RedBlackTree rbTree;
    simulateBattle(rbTree, numRobots, initialPositions, rounds);
  }
  else if (gameMode == 2)
  {
    // Modo Simulación
    RedBlackTree rbTree;

    // Configuración de la batalla
    int numRobots;
    std::cout << "Seleccione la categoría de la batalla:\n"
              << "1. 1vs1\n"
              << "2. 2vs2\n"
              << "3. 3vs3\n"
              << "4. 4vs4\n"
              << "5. 5vs5\n"
              << "Opción: ";
    int option;
    std::cin >> option;

    int rounds;
    std::cout << "Ingrese el número de rounds (máximo 10): ";
    std::cin >> rounds;

    if (rounds < 1 || rounds > MAX_ROUNDS)
    {
      std::cout << "Número inválido de rounds. Terminando el programa.\n";
      return 1;
    }

    switch (option)
    {
    case 1:
      numRobots = 2;
      break;
    case 2:
      numRobots = 4;
      break;
    case 3:
      numRobots = 6;
      break;
    case 4:
      numRobots = 8;
      break;
    case 5:
      numRobots = 10;
      break;
    default:
      std::cerr << "Opción no válida. Saliendo del programa.\n";
      return 1;
    }

    // Simulación de la batalla
    simulateBattle(rbTree, numRobots, rounds);
  }
  else
  {
    std::cerr << "Opción no válida. Saliendo del programa.\n";
    return 1;
  }

  return 0;
}
