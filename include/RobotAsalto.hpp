#ifndef ROBOT_ASALTO_HPP
#define ROBOT_ASALTO_HPP

#include "Robot.hpp"

class RobotAsalto : public Robot {
public:
    RobotAsalto(int id, const std::string& name, int energy, int x, int y);
    void move(int newX, int newY) override;
    void attack(Robot& target) override;
    void recharge(int energy) override;
    void substract(int energy) override;
};

#endif
