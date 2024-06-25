#ifndef RED_BLACK_TREE_HPP
#define RED_BLACK_TREE_HPP

#include "Robot.hpp"
#include <memory>
#include <functional>

class RedBlackTree
{
public:
    RedBlackTree();
    ~RedBlackTree();
    void insert(std::shared_ptr<Robot> robot);
    void remove(int robotID);
    std::shared_ptr<Robot> find(int robotID);
    void printInOrder();
    void forEach(const std::function<void(std::shared_ptr<Robot>)> &func) const;
    int size() const;

private:
    struct Node
    {
        std::shared_ptr<Robot> robot;
        bool color; // Red = true, Black = false
        std::shared_ptr<Node> left, right, parent;

        Node(std::shared_ptr<Robot> robot)
            : robot(robot), color(true), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    std::shared_ptr<Node> root;

    void clearHelper(std::shared_ptr<Node> node);
    void rotateLeft(std::shared_ptr<Node> node);
    void rotateRight(std::shared_ptr<Node> node);
    void fixInsert(std::shared_ptr<Node> node);
    void fixRemove(std::shared_ptr<Node> node);
    void transplant(std::shared_ptr<Node> u, std::shared_ptr<Node> v);
    std::shared_ptr<Node> minimum(std::shared_ptr<Node> node);
    void printInOrderHelper(std::shared_ptr<Node> node);
    void forEachHelper(std::shared_ptr<Node> node, const std::function<void(std::shared_ptr<Robot>)> &func) const;
};

#endif
