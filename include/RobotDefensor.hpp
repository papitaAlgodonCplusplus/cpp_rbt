#ifndef ROBOT_DEFENSOR_HPP
#define ROBOT_DEFENSOR_HPP

#include "Robot.hpp"

class RobotDefensor : public Robot {
public:
    RobotDefensor(int id, const std::string& name, int energy, int x, int y);
    void move(int newX, int newY) override;
    void attack(Robot& target) override;
    void recharge(int energy) override;
    void substract(int energy) override;
    void transferEnergy(Robot& target) override;
    void defender(Robot& target);
    bool defending = false;
};

#endif
