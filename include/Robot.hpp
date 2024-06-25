#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <string>
#include <utility> // For std::move
#include <iostream> //For standard output

class Robot {
public:
    Robot(int id, const std::string& name, const std::string& type, int energy, int x, int y);
    // Destructor
    virtual ~Robot() = default;

    // Copy constructor
    Robot(const Robot& other);

    // Copy assignment operator
    Robot& operator=(const Robot& other);

    // Move constructor
    Robot(Robot&& other) noexcept;

    // Move assignment operator
    Robot& operator=(Robot&& other) noexcept;

    int getID() const;
    const std::string& getName() const;
    const std::string& getType() const;
    bool isInRange(const Robot& target, int range) const;
    const int& getX() const;
    const int& getY() const;
    void setDefended(bool x);
    int getEnergyLevel() const;
    std::pair<int, int> getPosition() const;

    virtual void move(int newX, int newY) = 0;
    virtual void attack(Robot& target) = 0;
    virtual void recharge(int energy) = 0;
    virtual void substract(int energy) = 0;
    virtual void transferEnergy(Robot& target) = 0;
    void setTeam(int team) {this->team = team;};
    int getTeam() const {return this->team;};

protected:
    int id;
    std::string name;
    std::string type;
    int energyLevel;
    int posX;
    int posY;
    int team;
    bool defended = false;
};

#endif
