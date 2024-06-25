#include "RedBlackTree.hpp"
#include <iostream>

// Constructor de RedBlackTree
RedBlackTree::RedBlackTree() : root(nullptr) {}

// Implementación de forEach
void RedBlackTree::forEach(const std::function<void(std::shared_ptr<Robot>)>& func) const {
    forEachHelper(root, func);
}

// Helper para recorrer el árbol en orden y aplicar la función
void RedBlackTree::forEachHelper(std::shared_ptr<Node> node, const std::function<void(std::shared_ptr<Robot>)>& func) const {
    if (node != nullptr) {
        forEachHelper(node->left, func);
        func(node->robot);
        forEachHelper(node->right, func);
    }
}

int RedBlackTree::size() const {
    int count = 0;
    std::function<void(std::shared_ptr<Node>)> countNodes = [&](std::shared_ptr<Node> node) {
        if (node) {
            countNodes(node->left);
            count++;
            countNodes(node->right);
        }
    };

    countNodes(root);
    return count;
}

// Función auxiliar para realizar una rotación a la izquierda
void RedBlackTree::rotateLeft(std::shared_ptr<Node> x) {
    auto y = x->right;
    x->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// Función auxiliar para realizar una rotación a la derecha
void RedBlackTree::rotateRight(std::shared_ptr<Node> y) {
    auto x = y->left;
    y->left = x->right;
    if (x->right != nullptr) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == nullptr) {
        root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

// Función para arreglar las propiedades después de la inserción
void RedBlackTree::fixInsert(std::shared_ptr<Node> k) {
    std::shared_ptr<Node> u;
    while (k->parent && k->parent->color == true) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;
            if (u && u->color == true) {
                u->color = false;
                k->parent->color = false;
                k->parent->parent->color = true;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rotateRight(k);
                }
                k->parent->color = false;
                k->parent->parent->color = true;
                rotateLeft(k->parent->parent);
            }
        } else {
            u = k->parent->parent->right;
            if (u && u->color == true) {
                u->color = false;
                k->parent->color = false;
                k->parent->parent->color = true;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    rotateLeft(k);
                }
                k->parent->color = false;
                k->parent->parent->color = true;
                rotateRight(k->parent->parent);
            }
        }
        if (k == root) {
            break;
        }
    }
    root->color = false;
}

// Función para arreglar las propiedades después de la eliminación
void RedBlackTree::fixRemove(std::shared_ptr<Node> x) {
    while (x != root && x->color == false) {
        if (x == x->parent->left) {
            auto w = x->parent->right;
            if (w->color == true) {
                w->color = false;
                x->parent->color = true;
                rotateLeft(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == false && w->right->color == false) {
                w->color = true;
                x = x->parent;
            } else {
                if (w->right->color == false) {
                    w->left->color = false;
                    w->color = true;
                    rotateRight(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = false;
                w->right->color = false;
                rotateLeft(x->parent);
                x = root;
            }
        } else {
            auto w = x->parent->left;
            if (w->color == true) {
                w->color = false;
                x->parent->color = true;
                rotateRight(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == false && w->left->color == false) {
                w->color = true;
                x = x->parent;
            } else {
                if (w->left->color == false) {
                    w->right->color = false;
                    w->color = true;
                    rotateLeft(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = false;
                w->left->color = false;
                rotateRight(x->parent);
                x = root;
            }
        }
    }
    if (x != nullptr) x->color = false;
}

// Función para insertar un nodo en el árbol rojo-negro
void RedBlackTree::insert(std::shared_ptr<Robot> robot) {
    auto node = std::make_shared<Node>(robot);
    node->parent = nullptr;
    node->robot = robot;
    node->left = nullptr;
    node->right = nullptr;
    node->color = true;

    auto y = std::shared_ptr<Node>(nullptr);
    auto x = root;

    while (x != nullptr) {
        y = x;
        if (node->robot->getID() < x->robot->getID()) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    node->parent = y;
    if (y == nullptr) {
        root = node;
    } else if (node->robot->getID() < y->robot->getID()) {
        y->left = node;
    } else {
        y->right = node;
    }

    if (node->parent == nullptr) {
        node->color = false;
        return;
    }

    if (node->parent->parent == nullptr) {
        return;
    }

    fixInsert(node);
}

// Función auxiliar para encontrar el nodo mínimo
std::shared_ptr<RedBlackTree::Node> RedBlackTree::minimum(std::shared_ptr<Node> node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Función auxiliar para reemplazar nodos en el árbol
void RedBlackTree::transplant(std::shared_ptr<Node> u, std::shared_ptr<Node> v) {
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != nullptr) {
        v->parent = u->parent;
    }
}

// Función para eliminar un nodo del árbol rojo-negro
void RedBlackTree::remove(int robotID) {
    auto z = root;
    std::shared_ptr<Node> x, y;

    while (z != nullptr && z->robot->getID() != robotID) {
        if (robotID < z->robot->getID()) {
            z = z->left;
        } else {
            z = z->right;
        }
    }

    if (z == nullptr) {
        return; // No se encontró el nodo a eliminar
    }

    y = z;
    auto yOriginalColor = y->color;
    if (z->left == nullptr) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == nullptr) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x != nullptr) {
                x->parent = y;
            }
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (yOriginalColor == false && x != nullptr) {
        fixRemove(x);
    }
}

// Función para encontrar un robot en el árbol por su ID
std::shared_ptr<Robot> RedBlackTree::find(int robotID) {
    auto node = root;
    while (node != nullptr) {
        if (robotID == node->robot->getID()) {
            return node->robot;
        } else if (robotID < node->robot->getID()) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return nullptr; // No se encontró el robot
}

// Función para imprimir el árbol en orden
void RedBlackTree::printInOrder() {
    printInOrderHelper(root);
}

// Función auxiliar para imprimir el árbol en orden
void RedBlackTree::printInOrderHelper(std::shared_ptr<Node> node) {
    if (node != nullptr) {
        printInOrderHelper(node->left);
        std::cout << "Robot ID: " << node->robot->getID() << ", Name: " << node->robot->getName() << std::endl;
        printInOrderHelper(node->right);
    }
}
