#include "RobotExplorador.hpp"
#include <iostream>
#include <random>

RobotExplorador::RobotExplorador(int id, const std::string& name, int energy, int x, int y)
    : Robot(id, name, "Explorador", energy, x, y) {}

void RobotExplorador::move(int newX, int newY) {
    int distance = abs(newX - posX) + abs(newY - posY);
    int energyRequired = distance * 5;
    if (energyLevel >= energyRequired) {
        posX = newX;
        posY = newY;
        energyLevel -= energyRequired;
        std::cout << name << " moved to (" << posX << ", " << posY << ").\n";
        std::cout << name << " energy is now: " << energyLevel << ".\n";
    } else {
        std::cout << "Not enough energy to move.\n";
    }
}

void RobotExplorador::attack(Robot& target) {
    if (energyLevel >= 20) {
        energyLevel -= 20;
        std::cout  << name << " attacked " << target.getName() << ".\n";
        target.substract(20);
        std::cout << name << " energy is now: " << energyLevel << ".\n";
    } else {
        std::cout << "Not enough energy to attack.\n";
    }
}

void RobotExplorador::substract(int energy) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);
    if(dis(gen) > 50) {
        std::cout << name << " evaded attack." << "\n";
        energyLevel -= 15;
        std::cout << name << " energy is now: " << energyLevel << ".\n";
        return;
    }

    energyLevel -= energy;
    std::cout << name << " lost " << energy << " units of energy. Current energy: " << energyLevel << "\n";
}

void RobotExplorador::recharge(int energy) {
    energyLevel += energy;
    std::cout << name << " recharged " << energy << " units of energy. Current energy: " << energyLevel << "\n";
}

