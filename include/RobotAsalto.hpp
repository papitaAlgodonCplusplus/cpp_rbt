#ifndef ROBOT_ASALTO_HPP
#define ROBOT_ASALTO_HPP

#include "Robot.hpp"

class RobotAsalto : public Robot {
public:
    RobotAsalto(int id, const std::string& name, int energy, int x, int y);
    // Destructor
    ~RobotAsalto() override = default;

    // Copy constructor
    RobotAsalto(const RobotAsalto& other);

    // Copy assignment operator
    RobotAsalto& operator=(const RobotAsalto& other);

    // Move constructor
    RobotAsalto(RobotAsalto&& other) noexcept;

    // Move assignment operator
    RobotAsalto& operator=(RobotAsalto&& other) noexcept;

    void move(int newX, int newY) override;
    void attack(Robot& target) override;
    void recharge(int energy) override;
    void substract(int energy) override;
    void transferEnergy(Robot& target) override;
};

#endif
