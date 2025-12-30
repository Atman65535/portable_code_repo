//
// Created by atman on 10/13/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
    double index;
    double value;
    struct node *next;
}node, * p_node;

/**
 * @brief  build a new chain
 * @return a new chain list, with an empty root node
 */
p_node build_chain() {
    p_node n = (p_node)malloc(sizeof(node));
    n->next = NULL;
    n->index = 0;
    n->value = 0;
    return n;
}
// insert node_agter to node_before.
void add_node_after(p_node node_before, p_node node_after){
    node_after->next = node_before->next;
    node_before->next = node_after;
}
// delete next node after node_before_delete
bool del_node(p_node node_before_delete){
    if(node_before_delete->next == NULL) return false; // no delete
    p_node temp_for_free = node_before_delete->next;
    node_before_delete->next = (node_before_delete->next)->next;
    free(temp_for_free);
}

void arbit(p_node head_node){

}

// freeee the whole chain recrusively ? (actually not recrusive)
void free_chain(p_node head_node){
    p_node tmp;
    while(head_node->next != NULL){
        tmp = head_node;
        head_node = head_node->next;
        free(tmp);
    }
    free(head_node);
}

p_node add_poly(p_node first_poly, p_node next_poly){
    // empty circumstances(really no use. Who will add two empty ? but I cannot skip this segment)
    if(first_poly->next == NULL)
    return next_poly;
    if(next_poly->next == NULL)
    return first_poly;

    p_node insert_curser = first_poly;
    p_node candidate_first = first_poly->next;
    p_node candidate_second = next_poly->next;
    while(candidate_first != NULL && candidate_second !=NULL){
        // while there is something to add
        if(candidate_first->index == candidate_second->index){
            // if two idx are same, then add them
            candidate_first->value = candidate_first->value + candidate_second->value;

            candidate_first = candidate_first->next;
            candidate_second = candidate_second->next;
            // remember to push the insert place
            insert_curser = insert_curser->next;
        }
        else{
            if(candidate_first->index > candidate_second->index){
                candidate_first = candidate_first->next;
                insert_curser = insert_curser->next;
            }
            else{
                p_node node_to_ins = (p_node)malloc(sizeof(node));
                node_to_ins->next = candidate_first;
                node_to_ins->index = candidate_second->index;
                node_to_ins->value = candidate_second->value;
                candidate_second = candidate_second->next;
                // remember to push the curser
                add_node_after(insert_curser, node_to_ins);
                insert_curser = insert_curser->next;
            }
        }
    }
    if(candidate_first == NULL && candidate_second)
      add_node_after(insert_curser, candidate_second);
    return first_poly;
}

void visualize_polynomial(p_node chain_list){
    p_node cur = chain_list->next;
    printf("%.2fx^%.0f", cur->value, cur->index);
    cur = cur->next;
    while(cur){
        printf("%+.2fx^%.0f", cur->value, cur->index);
        cur = cur->next;
    }
    putc('\n', stdout);
}

typedef struct bined_val{
    double val;
    double index;
}val;

int cmp(const void* a, const void* b){
    val* x = (val*)a;
    val* y = (val*)b;
    if(x->index > y->index)
    return -1;
    if(x->index < y->index)
    return 1;
    return 0;
}
void ins_to_head(p_node head, int n, val* vals) {
    for (int i = n-1; i >=0 ;i --) {
        p_node n = (p_node)malloc(sizeof(node));
        n->index = vals[i].index;
        n->value = vals[i].val;
        n->next = NULL;
        add_node_after(head, n);
    }
}

p_node add_with_preserved_chain(p_node first_poly, p_node next_poly) {
    if(first_poly->next == NULL)
        return next_poly;
    if(next_poly->next == NULL)
        return first_poly;

    p_node new_chain = build_chain();
    p_node insert_curser = new_chain;
    p_node candidate_first = first_poly->next;
    p_node candidate_second = next_poly->next;
    while(candidate_first != NULL && candidate_second !=NULL){
        // while there is something to add
        if(candidate_first->index == candidate_second->index){
            // if two idx are same, then add them
            p_node node_to_ins = (p_node)malloc(sizeof(node));
            node_to_ins->index = candidate_first->index;
            node_to_ins->value = candidate_first->value + candidate_second->value;
            node_to_ins->next = NULL;

            candidate_first = candidate_first->next;
            candidate_second = candidate_second->next;
            add_node_after(insert_curser, node_to_ins);
            // remember to push the insert place
            insert_curser = insert_curser->next;
        }
        else{
            if(candidate_first->index > candidate_second->index){
                p_node node_to_ins = (p_node)malloc(sizeof(node));
                node_to_ins->index = candidate_first->index;
                node_to_ins->value = candidate_first->value;
                node_to_ins->next = NULL;

                add_node_after(insert_curser, node_to_ins);
                candidate_first = candidate_first->next;
                insert_curser = insert_curser->next;
            }
            else{
                p_node node_to_ins = (p_node)malloc(sizeof(node));
                node_to_ins->next = NULL;
                node_to_ins->index = candidate_second->index;
                node_to_ins->value = candidate_second->value;

                candidate_second = candidate_second->next;
                // remember to push the curser
                add_node_after(insert_curser, node_to_ins);
                insert_curser = insert_curser->next;
            }
        }
    }
    if(candidate_first == NULL && candidate_second)
        add_node_after(insert_curser, candidate_second);
    else if (candidate_first != NULL && candidate_second == NULL)
        add_node_after(insert_curser, candidate_first);
    return new_chain;
}

int main() {
    p_node poly_first = build_chain();
    p_node poly_second = build_chain();

    // if the index in not in a proper sequence
    // I really don't know the input format, so I guess like below
    val original_val[100];
    int n; // the count of couple
    scanf("%d",&n);
    for (int i =0 ; i< n; i++) {
        scanf("%lf,%lf", &original_val[i].index, &original_val[i].val);
    }
    qsort(original_val, n, sizeof(val), cmp);
    ins_to_head(poly_first, n, original_val);

    scanf("%d", &n);
    for (int i =0 ; i< n; i++) {
        scanf("%lf,%lf", &original_val[i].index, &original_val[i].val);
    }
    qsort(original_val, n, sizeof(val), cmp);
    ins_to_head(poly_second, n, original_val);

    visualize_polynomial(add_with_preserved_chain(poly_first,poly_second));
    add_poly(poly_first, poly_second);
    visualize_polynomial(poly_first);


    free_chain(poly_first);
    free_chain(poly_second);
}

