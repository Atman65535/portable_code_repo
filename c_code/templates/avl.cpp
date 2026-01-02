//
// Created by atman on 1/2/26.
//
#include "avl.hpp"

int main() {
    AVLTree<int> tree;
    int data[] = {10, 20, 40, 50, 70, 30};
    for (int x:data){
        tree.ins(x);
    }
    std::cout << "BST check: ";
    tree.print_inorder();
    tree.rm(40);
    std::cout << "tree after del 40: ";
    tree.print_inorder();
    return 0;
}