//
// 2026.1.2 Author atman
//

#include <array>
#include <iostream>

template <typename T> class RBTree {
private:
  enum Color { RED, BLACK };

  struct Node {
    T data;
    Color color;
    Node *left;
    Node *right;
    Node *parent;

    Node(T data)
        : data(data), color(RED), left(nullptr), right(nullptr),
          parent(nullptr) {}
  };
  Node* root = nullptr;

  void left_rotate(Node *father) {
    Node *son = father->right;
    father->right = son->left;
    if (son->left != nullptr)
      son->left->parent = father;
    son->parent = father->parent;
    if (father->parent == nullptr) // root
      root = son;
    else if (father == father->parent->left)
      father->parent->left = son;
    else
      father->parent->right = son;
    son->left = father;
    father->parent = son;
  }

  void right_rotate(Node *father) {
    Node *son = father->left;
    father->left = son->right;
    if (son->right != nullptr)
      son->right->parent = father;
    son->parent = father->parent;
    if (father->parent == nullptr) // root
      root = son;
    else if (father == father->parent->left)
      father->parent->left = son;
    else
      father->parent->right = son;
    son->right = father;
    father->parent = son;
  }
  /*
   * @brief this function maintains the color distrubutio after
   *        insert operation
   *        the grandpa node must be black.
   *        All conflicts come from father and son RED
   * @param node this parameter is the inserted node. RED by default
   */
  void fix_insert(Node *node) {
    while (node != root && node->parent->color == RED) {
      // instance 1 uncle on right, like LL/ LR
      if (node->parent == node->parent->parent->left) {
        Node *uncle = node->parent->parent->right;
        if (uncle != nullptr && uncle->color == RED) {
          // typq 1 uncle red, change color
          node->parent->color = BLACK;
          uncle->color = BLACK;
          node->parent->parent->color = RED;
          node = node->parent->parent;
        }
        else {
          if (node == node->parent->right) {
            // LR type, left rotate parent first
            // convert to LL type
            node = node->parent;
            left_rotate(node);
          }
          node->parent->color = BLACK;
          node->parent->parent->color = RED;
          right_rotate(node->parent->parent);
        }
      }
      else {
        // the dual circumstance
        Node* uncle = node->parent->parent->left;
        if (uncle != nullptr && uncle->color == RED) {
          uncle->color = BLACK;
          node->parent->color = BLACK;
          node->parent->parent->color = RED;
        }
        else {
          // uncle is Black node
          if (node == node->parent->left) {
            // RL type, right rotate parent first
            // convert to RR type
            node = node->parent;
            right_rotate(node);
          }
          node->parent->parent->color = RED;
          node->parent->color = BLACK;
          left_rotate(node->parent->parent);
        }
      } 
    }
    root->color = BLACK;
  }

  void in_order(Node* node) {
    if (node == nullptr) return;
    in_order(node->left);
    std::cout << node->data << "(" << (node->color == RED ? "R":"B") << ")";
    in_order(node->right);
  }
public:
  RBTree() : root(nullptr) {}

  void insert(T data) {
    Node* node = new Node(data);
    Node* y = nullptr;
    Node* x = root;

    while (x != nullptr) {
      y = x;
      if (node->data < x->data) {
        x = x->left;
      }
      else x = x->right;
    }
    node->parent = y;
    if (y == nullptr) {
      root = node;
    }
    else if (node->data > y->data)
      y->right = node;
    else y->left = node;

    this->fix_insert(node);
  }

  void print_tree() {
    in_order(root);
    std::cout << std::endl;
  }
};


int main() {
  RBTree<int> tree;
  std::array<int, 10> arr = {10, 30, 22, 32, 12, 43, 54, 78, 86, 14};

  std::cout << "Inserting Elements..." << std::endl;
  for (const auto& x: arr) {
    tree.insert(x);
  }
  std::cout << "In order Traversal " << std::endl;
  tree.print_tree();
}
