//
// Created by atman on 10/13/25.
//
#include <iostream>
#include <vector>
#include <array>
#include <atomic>
#include <fstream>

using std::vector;
using std::string, std::array, std::cout, std::endl;
using std::ifstream, std::ofstream;
array<array<int, 11>, 7> maze={
    {
        {1,1,1,1,1,1,1,1,1,1,1},
        {1,1,8,9,7,7,8,9,2,2,1},
        {1,2,7,8,8,8,8,9,1,1,2},
        {1,1,1,8,8,8,7,8,1,1,1},
        {2,2,2,7,7,8,9,8,1,1,1},
        {1,1,1,2,7,7,8,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1},
    }
};

array<array<int, 2>, 8> directions= {
    {
        {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}
    }
};



class ArrayStack {
public:
    ArrayStack() = default;
    ~ArrayStack() = default;
    typedef struct coord {
        int row, col;
    }coordinate;
    bool is_empty() {
        return stack.empty();
    }
    void push(coordinate val) {
        stack.push_back(val);
    }
    coordinate pop() {
        coordinate last = stack.back();
        stack.pop_back();
        return last;
    }
    array<array<int , 11>, 7> seed_fill(const array<array<int,11>, 7>& maze_func, int seed_row, int seed_col, bool visualize = false) {
        array<array<int , 11>, 7> maze_flag{};
        push(coordinate(seed_row, seed_col));
        maze_flag[seed_row][seed_col] = 1;
        while (!is_empty()) {
            coordinate center = pop();
            for (int i = 0 ; i < 8 ;i++) {
                coordinate temp;
                temp.col = center.col + directions[i][0];
                temp.row = center.row + directions[i][1];
                if (temp.col >=0 && temp.col <11) {
                    if (temp.row >=0 && temp.row < 7) {
                        if (maze_flag[temp.row][temp.col] == 0) {
                            if (std::abs(maze_func.at(temp.row).at(temp.col) - maze_func.at(center.row).at(center.col)) <=1) {
                                push(temp);
                                maze_flag[temp.row][temp.col] = 1;
                            }
                        }
                    }
                }
            }
            // for (int i = 0;i<7;i++) {
            //     for (int j =0; j<11;j++) {
            //         cout << maze_flag[i][j] << ' ';
            //     }
            //     cout <<endl;
            // }
        }
        if (visualize) {
            for (int i =0 ; i<7;i++) {
                for (int j=0;j<11;j++) {
                    cout << maze_flag[i][j] << ' ';
                }
                cout << '\n';
            }
        }
        return maze_flag;
    }
private:
    std::vector<coordinate> stack;
};


class ChainStack {
public:
    typedef struct coord {
        int row, col;
    }coordinate;
    ChainStack() {
        head = new node;
        tail = head; // this means tail is the last container
    }
    // delete all nodes
    ~ChainStack() {
        node_ptr cur = tail;
        while (cur != head) {
            const node_ptr tmp = cur;
            cur = cur->predecessor;
            delete tmp;
        }
        delete tail;
    }
    void push(coordinate val) {
        node_ptr new_node = build_new_node(val, tail);
        tail = new_node;
    }
    coordinate pop() {
        coordinate val = tail->value;
        node_ptr tmp = tail;
        tail = tail->predecessor;
        delete tmp;
        return val;
    }
    coordinate check_last() {
        return tail->value;
    }
    bool is_empty() {
        return head == tail;
    }

    array<array<int , 11>, 7> seed_fill(const array<array<int,11>,7>& maze_func,int seed_row,
        int seed_col,
        bool visualize) {
        array<array<int , 11>, 7> maze_flag{};
        push(coordinate(seed_row, seed_col));
        maze_flag[seed_row][seed_col] = 1;
        while (!is_empty()) {
            coordinate center = pop();
            for (int i = 0 ; i < 8 ;i++) {
                coordinate temp;
                temp.col = center.col + directions[i][0];
                temp.row = center.row + directions[i][1];
                if (temp.col >=0 && temp.col <11) {
                    if (temp.row >=0 && temp.row < 7) {
                        if (maze_flag[temp.row][temp.col] == 0) {
                            if (std::abs(maze_func.at(temp.row).at(temp.col) - maze_func.at(center.row).at(center.col)) <=1) {
                                push(temp);
                                maze_flag[temp.row][temp.col] = 1;
                            }
                        }
                    }
                }
            }
        }
        if (visualize) {
            for (int i =0 ; i<7;i++) {
                for (int j=0;j<11;j++) {
                    cout << maze_flag[i][j] << ' ';
                }
                cout << '\n';
            }
        }
        return maze_flag;
    }


private:
    typedef struct node{
        coordinate value;
        struct node * predecessor;
    }node, * node_ptr;
    node_ptr head = nullptr;
    node_ptr tail = nullptr;

    node_ptr build_new_node(coordinate val = coordinate(0,0), node_ptr predecessor = nullptr) {
        auto node_new = new node;
        node_new->value = val;
        node_new->predecessor = predecessor;
        return node_new;
    }
};

bool check(const array<array<int, 11>, 7> & maze1, const array<array<int, 11>, 7> & maze2) {
    for (int i = 0 ; i < 7 ; i++) {
        for (int j=0;j<11;j++) {
            if (maze1[i][j] != maze2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    ofstream file_out("out.bin",std::ios::binary);
    file_out.write(reinterpret_cast<char*>(&maze), sizeof(maze));

    ifstream file_in("out.bin", std::ios::binary);
    file_in.read(reinterpret_cast<char*>(&maze),sizeof(array<array<int, 11>, 7>));

    cout << "original \n";
    for (int i = 0;i<7; i++) {
        for (int j=0; j< 11;j++) {
            cout << maze[i][j] << ' ';
        }
        cout << endl; // cout << '\n'
    }
    int seed_x, seed_y;
    std::cin  >> seed_x;
    std::cin >> seed_y;

    ArrayStack stack;
    ChainStack chain_stack;
    bool visualize = true;
    cout << "the result of array stack" << endl;
    array<array<int, 11>, 7> maze_flag1 = stack.seed_fill(maze, seed_x, seed_y, visualize);
    cout << "the result of chain stack" << endl;
    array<array<int, 11>, 7> maze_flag2 = chain_stack.seed_fill(maze, seed_x, seed_y, visualize);

    if (check(maze_flag1, maze_flag2)) {
        cout << "checked pass !" << endl;
    }
    else {
        cout << "fatal error, the two results are not match !" << endl;
    }
    return 0;
}

