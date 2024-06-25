#ifndef ROBOT_EXPLORADOR_HPP
#define ROBOT_EXPLORADOR_HPP

#include "Robot.hpp"

class RobotExplorador : public Robot {
public:
    RobotExplorador(int id, const std::string& name, int energy, int x, int y);
    void move(int newX, int newY) override;
    void attack(Robot& target) override;
    void recharge(int energy) override;
    void substract(int energy) override;
};

#endif
