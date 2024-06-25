#include "RobotExplorador.hpp"
#include <iostream>
#include <random>

RobotExplorador::RobotExplorador(int id, const std::string& name, int energy, int x, int y)
    : Robot(id, name, "Explorador", energy, x, y) {}

RobotExplorador::RobotExplorador(const RobotExplorador& other)
    : Robot(other)  // Call base class copy constructor
{
}

RobotExplorador& RobotExplorador::operator=(const RobotExplorador& other)
{
    if (this != &other) {
        Robot::operator=(other);  // Call base class copy assignment operator
        // Any additional member variables specific to RobotExplorador can be assigned here
    }
    return *this;
}

RobotExplorador::RobotExplorador(RobotExplorador&& other) noexcept
    : Robot(std::move(other))  // Call base class move constructor
{
    // Any additional member variables specific to RobotExplorador can be moved here
}

RobotExplorador& RobotExplorador::operator=(RobotExplorador&& other) noexcept
{
    if (this != &other) {
        Robot::operator=(std::move(other));  // Call base class move assignment operator
        // Any additional member variables specific to RobotExplorador can be moved here
    }
    return *this;
}

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

void RobotExplorador::transferEnergy(Robot& target) {
    if (energyLevel >= 25) {
        energyLevel -= 25;
        std::cout << name << " transferred 20 energy to " << target.getName() << ".\n";
        target.recharge(20);
        std::cout << name << " energy is now: " << energyLevel << ".\n";
    } else {
        std::cout << "Not enough energy to transfer.\n";
    }
}