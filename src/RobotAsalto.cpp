#include "RobotAsalto.hpp"
#include <iostream>
#include <random>

RobotAsalto::RobotAsalto(int id, const std::string &name, int energy, int x, int y)
    : Robot(id, name, "Asalto", energy, x, y) {}

RobotAsalto::RobotAsalto(const RobotAsalto& other)
    : Robot(other)  // Call base class copy constructor
{
}

RobotAsalto& RobotAsalto::operator=(const RobotAsalto& other)
{
    if (this != &other) {
        Robot::operator=(other);  // Call base class copy assignment operator
    }
    return *this;
}

RobotAsalto::RobotAsalto(RobotAsalto&& other) noexcept
    : Robot(std::move(other))  // Call base class move constructor
{
}

RobotAsalto& RobotAsalto::operator=(RobotAsalto&& other) noexcept
{
    if (this != &other) {
        Robot::operator=(std::move(other));  // Call base class move assignment operator
    }
    return *this;
}

void RobotAsalto::move(int newX, int newY)
{
    int distance = abs(newX - posX) + abs(newY - posY);
    int energyRequired = distance * 10;
    if (energyLevel >= energyRequired)
    {
        posX = newX;
        posY = newY;
        energyLevel -= energyRequired;
        std::cout << name << " moved to (" << posX << ", " << posY << ").\n";
        std::cout << name << " energy is now: " << energyLevel << ".\n";
    }
    else
    {
        std::cout << "Not enough energy to move.\n";
    }
}

#include "RobotAsalto.hpp"
#include <iostream>

void RobotAsalto::attack(Robot &target)
{
    if (energyLevel >= 30)
    {
        std::cout << name << " attacked " << target.getName() << ".\n";
        target.substract(30); // Reducir la energía en 30 unidades
        std::cout << name << " energy is now: " << energyLevel << ".\n";
    }
    else
    {
        std::cout << "Not enough energy to attack.\n";
    }
}

void RobotAsalto::substract(int energy)
{
    energyLevel -= energy;
    std::cout << name << " lost " << energy << " units of energy. Current energy: " << energyLevel << "\n";
}

void RobotAsalto::recharge(int energy)
{
    energyLevel += energy;
    std::cout << name << " recharged " << energy << " units of energy. Current energy: " << energyLevel << "\n";
}

void RobotAsalto::transferEnergy(Robot& target) {
    if (energyLevel >= 25) {
        energyLevel -= 25;
        std::cout << name << " transferred 20 energy to " << target.getName() << ".\n";
        target.recharge(20);
        std::cout << name << " energy is now: " << energyLevel << ".\n";
    } else {
        std::cout << "Not enough energy to transfer.\n";
    }
}