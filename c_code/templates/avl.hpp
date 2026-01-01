// here is some tips and notes
//
// AVL tree is a kind of balance binary tree
// LL, RR, LR, RL four kinds of rotation options.
// LL, right single rotation
// RR, left single rotation. cuz this operation distract one depth
// LR, first singel left rotation on father node
//     then become LL type, right rotation on grandpa
// RL, that first single right rotation on father node
//     then become RR type left rotation on grandpa.

#include <iostream>
#include <string>
#include <vector>
#include <memory>

template<typename T>
class AVLTree{
private:
    struct Node {
        T key;
        Node* left;
        Node* right;
        int height;

        Node(const T& key)
            : key(key), left(nullptr), right(nullptr), height(0){}
    };

    Node* root = nullptr;

private:
    // tools function
    int height(const Node* node) const {
        return (node != nullptr)? node->height:0;
    }

    int balance_factor(const Node* node) const{
        // for safety, check the node if it is null
        return node ? (height(node->left) - height(node->right)):0;
    }

    void update_hight(const Node* node) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }

    Node* rotation_right(Node* current) {
        Node* left_son = current->left;

        current->left = left_son->right;
        left_son->right = current;
        update_hight(current);
        update_hight(left_son);
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
    
};
