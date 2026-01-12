#include <iostream>


typedef struct Node {
  int coef;
  int expn;
  Node* next;
}Node, *NodePtr;

/*
 * @brief add two chain together.
 *        we define that head node
 *        doesn't store data. 
 */
void add(NodePtr P, NodePtr Q) {
  NodePtr cur_p = P->next;
  NodePtr cur_q = Q->next;
  NodePtr pre_p = P;
  while (cur_q != nullptr && cur_p != nullptr) {
    if (cur_p->expn == cur_q->expn) {
      cur_p->coef = cur_p->coef + cur_q->coef;
      cur_q = cur_q->next;
      // delete
      if (cur_p->coef == 0) { 
        pre_p->next = cur_p->next;
        delete cur_p;
        cur_p = pre_p->next;
      }
      else {
        pre_p = cur_p;
        cur_p = cur_p->next;
      }
    }
    else if (cur_p->expn < cur_q->expn) {
      NodePtr q_copy = new Node;
      q_copy->expn = cur_q->expn;
      q_copy->coef = cur_q->coef;
      q_copy->next = cur_p;
      pre_p->next = q_copy;
      pre_p = pre_p->next;
      cur_q = cur_q->next;
    }
    else {
      pre_p = cur_p;
      cur_p = cur_p->next;
    }
  }
  while (cur_q != nullptr) {
     NodePtr tmpq = new Node;
     tmpq->coef = cur_q->coef;
     tmpq->expn = cur_q->expn;
     tmpq->next = nullptr;
     pre_p->next = tmpq;
     pre_p = pre_p->next;
     cur_q = cur_q->next;
  }
}


int main() {
  NodePtr P = new Node;
  NodePtr Q = new Node;
  int n;
  scanf("%d", &n);
  for (int i = 0; i< n; i++) {
    int coef, expn;
    std::cin >> expn >> coef;
    
  }
}
