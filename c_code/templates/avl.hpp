#pragma once
// here is some tips and notes
//
// AVL tree is a kind of balance binary tree
// LL, RR, LR, RL four kinds of rotation options.
// LL, right single rotation
// RR, left single rotation. cuz this operation distract one depth
// LR, first single left rotation on father node
//     then become LL type, right rotation on grandpa
// RL, that first single right rotation on father node
//     then become RR type left rotation on grandpa.

#include <iostream>
#include <string>
#include <vector>
#include <memory>

template <typename T>
class AVLTree {
private:
    struct Node {
        T key;
        Node* left;
        Node* right;
        int height;

        explicit Node(const T& key)
            : key(key), left(nullptr), right(nullptr), height(1) {
        }
    };

    Node* root = nullptr;

private:
    // tools function
    int height(const Node* node) const {
        return (node != nullptr) ? node->height : 0;
    }

    int balance_factor(const Node* node) const {
        // for safety, check the node if it is null
        return node ? (height(node->left) - height(node->right)) : 0;
    }

    void update_height(Node* node) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }

    Node* rotation_right(Node* current) {
        Node* left_son = current->left;

        current->left = left_son->right;
        left_son->right = current;
        update_height(current);
        update_height(left_son);
        return left_son;
    }

    /**
     * @brief easy that rotation left.
     */
    Node* rotation_left(Node* current) {
        Node* right_son = current->right;

        current->right = right_son->left;
        right_son->left = current;
        update_height(current);
        update_height(right_son);
        return right_son;
    }

    Node* insert(Node* node, const T& key) {
        if (!node)
            return new Node(key);
        if (key < node->key) {
            node->left = insert(node->left, key);
        }
        else if (key > node->key) {
            node->right = insert(node->right, key);
        }
        else
            return node;
        update_height(node);
        const int bf = balance_factor(node);
        // LL
        if (bf > 1 && key < node->left->key) {
            return rotation_right(node);
        }
        // LR
        if (bf > 1 && key > node->left->key) {
            node->left = rotation_left(node->left);
            return rotation_right(node);
        }
        // RR
        if (bf < -1 && key > node->right->key) {
            return rotation_left(node);
        }
        // RL
        if (bf < -1 && key < node->right->key) {
            node->right = rotation_right(node->right);
            return rotation_left(node);
        }
        return node;
    }

    Node* min_node(Node* node) {
        Node* cur = node;
        while (cur->left) {
            cur = cur->left;
        }
        return cur;
    }

    Node* remove(Node* node, const T& key) {
        if (!node)
            return nullptr;

        if (key < node->key) {
            node->left = remove(node->left, key);
        }
        else if (key > node->key) {
            node->right = remove(node->right, key);
        }
        // remove
        else {
            // 0 or 1 child. AVL trait: you can delete safely when this circumstance
            if (!node->left || !node->right) {
                Node* tmp = (node->left) ? node->left : node->right;
                delete node;
                return tmp;
            }
            // 2 child. Take the minimum one from right
            Node* min = min_node(node->right);
            node->key = min->key;
            node->right = remove(node->right, min->key);
        }
        update_height(node);
        const int bf = balance_factor(node);
        // LL
        if (bf > 1 && balance_factor(node->left) > 0) {
            return rotation_right(node);
        }
        // LR
        if (bf > 1 && balance_factor(node->left) < 0) {
            node->left = rotation_left(node->left);
            return rotation_right(node);
        }
        // RR
        if (bf < 1 && balance_factor(node->right) < 0) {
            return rotation_left(node);
        }
        // RL
        if (bf < 1 && balance_factor(node->right) > 0) {
            node->right = rotation_right(node->right);
            return rotation_left(node);
        }
        return node;
    }
    void inorder(Node* node) {
        if (!node) return;
        inorder(node->left);
        std::cout << node->key << " ";
        inorder(node->right);
    }

public:
    void ins(T& key) {
        root = this->insert(root, key);
    }
    void rm(const T& key) {
        root = this->remove(root, key);
    }
    void print_inorder() {
        this->inorder(root);
        std::cout << std::endl;
    }
};
