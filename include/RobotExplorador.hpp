#ifndef ROBOT_EXPLORADOR_HPP
#define ROBOT_EXPLORADOR_HPP

#include "Robot.hpp"

class RobotExplorador : public Robot {
public:
    RobotExplorador(int id, const std::string& name, int energy, int x, int y);
    // Destructor
    ~RobotExplorador() override = default;

    // Copy constructor
    RobotExplorador(const RobotExplorador& other);

    // Copy assignment operator
    RobotExplorador& operator=(const RobotExplorador& other);

    // Move constructor
    RobotExplorador(RobotExplorador&& other) noexcept;

    // Move assignment operator
    RobotExplorador& operator=(RobotExplorador&& other) noexcept;

    void move(int newX, int newY) override;
    void attack(Robot& target) override;
    void recharge(int energy) override;
    void substract(int energy) override;
    void transferEnergy(Robot& target) override;
};

#endif
