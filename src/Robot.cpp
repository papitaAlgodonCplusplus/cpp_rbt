#include "Robot.hpp"

Robot::Robot(int id, const std::string& name, const std::string& type, int energy, int x, int y)
    : id(id), name(name), type(type), energyLevel(energy), posX(x), posY(y) {}

int Robot::getID() const {
    return id;
}

const std::string& Robot::getName() const {
    return name;
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
