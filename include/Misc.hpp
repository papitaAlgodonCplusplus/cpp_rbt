#ifndef MISC_HPP
#define MISC_HPP

#include <utility>
#include <iostream>
#include <random>
#include <map>
#include <iomanip>
#include <memory>
#include <sstream>
#include "RedBlackTree.hpp"
#include "RobotAsalto.hpp"
#include "RobotExplorador.hpp"
#include "RobotDefensor.hpp"

// Constants
const int MAX_ROBOTS = 10;
const int MAX_ROUNDS = 10;
const int RECHARGE_ENERGY = 50;
const int FIELD_SIZE = 10;
const int MIN_POSITION = 0;
const int MAX_POSITION = FIELD_SIZE - 1;
const int ROWS = 10;
const int COLS = 10;

int randomRange(int min, int max);
std::shared_ptr<RobotDefensor> findActiveDefender(RedBlackTree &rbTree, const std::shared_ptr<Robot> &currentRobot);
int charToInt(char c);

// Función para imprimir el estado del tablero
void printBattlefield(const RedBlackTree &rbTree, int **rechargeStations, const int numRobots);

// Function to mark a position as available in the occupiedPositions array
int markPositionAvailable(int **occupiedPositions, int numRobots, int x, int y);

// Function to count occurrences of a specific position in the occupiedPositions array
int countPosition(int **occupiedPositions, int numRobots, int x, int y);

// Función para simular una batalla entre robots
void simulateBattle(RedBlackTree &rbTree, int numRobots, int **initialPositions, int rounds);

// Función para simular una batalla entre robots
void simulateBattle(RedBlackTree &rbTree, int numRobots, int rounds);

#endif
