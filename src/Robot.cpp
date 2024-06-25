#include "Robot.hpp"

Robot::Robot(int id, const std::string& name, const std::string& type, int energy, int x, int y)
    : id(id), name(name), type(type), energyLevel(energy), posX(x), posY(y) {}

int Robot::getID() const {
    return id;
}

const std::string& Robot::getName() const {
    return name;
}

// Copy constructor
Robot::Robot(const Robot& other)
    : id(other.id), name(other.name), type(other.type), energyLevel(other.energyLevel),
      posX(other.posX), posY(other.posY), team(other.team), defended(other.defended)
{
}

// Copy assignment operator
Robot& Robot::operator=(const Robot& other)
{
    if (this != &other) {
        id = other.id;
        name = other.name;
        type = other.type;
        energyLevel = other.energyLevel;
        posX = other.posX;
        posY = other.posY;
        team = other.team;
        defended = other.defended;
    }
    return *this;
}

// Move constructor
Robot::Robot(Robot&& other) noexcept
    : id(std::move(other.id)), name(std::move(other.name)), type(std::move(other.type)),
      energyLevel(std::move(other.energyLevel)), posX(std::move(other.posX)),
      posY(std::move(other.posY)), team(std::move(other.team)), defended(std::move(other.defended))
{
    // Reset the moved object's state
    other.id = 0;
    other.energyLevel = 0;
    other.posX = 0;
    other.posY = 0;
    other.team = 0;
    other.defended = false;
}

// Move assignment operator
Robot& Robot::operator=(Robot&& other) noexcept
{
    if (this != &other) {
        id = std::move(other.id);
        name = std::move(other.name);
        type = std::move(other.type);
        energyLevel = std::move(other.energyLevel);
        posX = std::move(other.posX);
        posY = std::move(other.posY);
        team = std::move(other.team);
        defended = std::move(other.defended);
        
        // Reset the moved object's state
        other.id = 0;
        other.energyLevel = 0;
        other.posX = 0;
        other.posY = 0;
        other.team = 0;
        other.defended = false;
    }
    return *this;
}

bool Robot::isInRange(const Robot& target, int range) const {
    int deltaX = target.getX() - posX;
    int deltaY = target.getY() - posY;
    return (deltaX * deltaX + deltaY * deltaY) <= (range * range);
}

void Robot::setDefended(bool x) {
    defended = x;
}

const int& Robot::getX() const {
  return posX;
}

const int& Robot::getY() const {
  return posY;
}

const std::string& Robot::getType() const {
    return type;
}

int Robot::getEnergyLevel() const {
    return energyLevel;
}

std::pair<int, int> Robot::getPosition() const {
    return {posX, posY};
}
