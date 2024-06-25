#include "RobotDefensor.hpp"
#include <iostream>

RobotDefensor::RobotDefensor(int id, const std::string& name, int energy, int x, int y)
    : Robot(id, name, "Defensor", energy, x, y) {}

RobotDefensor::RobotDefensor(const RobotDefensor& other)
    : Robot(other)  // Call base class copy constructor
{
    defending = other.defending;
}

RobotDefensor& RobotDefensor::operator=(const RobotDefensor& other)
{
    if (this != &other) {
        Robot::operator=(other);  // Call base class copy assignment operator
        defending = other.defending;
    }
    return *this;
}

RobotDefensor::RobotDefensor(RobotDefensor&& other) noexcept
    : Robot(std::move(other))  // Call base class move constructor
{
    defending = std::move(other.defending);
    other.defending = false;  // Reset moved object's state
}

RobotDefensor& RobotDefensor::operator=(RobotDefensor&& other) noexcept
{
    if (this != &other) {
        Robot::operator=(std::move(other));  // Call base class move assignment operator
        defending = std::move(other.defending);
        other.defending = false;  // Reset moved object's state
    }
    return *this;
}

void RobotDefensor::move(int newX, int newY) {
    int distance = abs(newX - posX) + abs(newY - posY);
    int energyRequired = distance * 10;
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

void RobotDefensor::attack(Robot& target) {
    if (energyLevel >= 25) {
        energyLevel -= 25;
        std::cout << name << " attacked " << target.getName() << ".\n";
        target.substract(25); // Reducir la energía en 25 unidades
        std::cout << name << " energy is now: " << energyLevel << ".\n";
    } else {
        std::cout << "Not enough energy to attack.\n";
    }
}

void RobotDefensor::recharge(int energy) {
    energyLevel += energy;
    std::cout << name << " recharged " << energy << " units of energy. Current energy: " << energyLevel << "\n";
}

void RobotDefensor::transferEnergy(Robot& target) {
    if (energyLevel >= 25) {
        energyLevel -= 25;
        std::cout << name << " transferred 20 energy to " << target.getName() << ".\n";
        target.recharge(20);
        std::cout << name << " energy is now: " << energyLevel << ".\n";
    } else {
        std::cout << "Not enough energy to transfer.\n";
    }
}

void RobotDefensor::substract(int energy) {
    energyLevel -= energy;
    std::cout << name << " lost " << energy << " units of energy. Current energy: " << energyLevel << "\n";
}

void RobotDefensor::defender(Robot& target) {
    if (energyLevel >= 20) {
        energyLevel -= 20;
        std::cout << name << " defending " << target.getName() << "\n";
        std::cout << name << " energy is now: " << energyLevel << ".\n";
    } else {
        std::cout << "Not enough energy to defend.\n";
    }
}
