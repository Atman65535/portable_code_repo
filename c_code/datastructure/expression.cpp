#include <cstddef>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <system_error>

class Stack {
private:
    struct Node{
        int data;
        Node* next;
    };
    Node* top = nullptr;
public:
    Stack() = default;
    void push_in(int data) {
        Node* cur = new Node({data, nullptr});
        cur->next = top;
        top = cur;
    }
    int pop(void) {
        int ret = top->data;
        Node* tmp = top;
        top = top->next;
        delete tmp;
        return ret;
    }
    bool is_empty(void) {
        return top == nullptr;
    }
    int gtop(void) {
        if (top == nullptr)
            throw(std::runtime_error("gtop a null !"));
        return top->data;
    }
};

int priority(int i) {
    if (i == '/')
        return 5;
    if (i == '*')
        return 3;
    if (i == '-')
        return 2;
    if (i == '+')
        return 1;
    if (i == '(')
        return 0;
}
int calc(int sign, int a, int b){
    if (sign == '+')
        return a + b;
    if (sign == '-')
        return a - b;
    if (sign == '*')
        return a*b;
    if (sign == '/')
        return a / b;
    else throw(std::runtime_error("invalid sing"));
}

void calc_one(Stack& numbers, Stack& signs) {
    int i2 = numbers.pop();
    int i1 = numbers.pop();
    numbers.push_in(calc(signs.pop(), i1, i2));
}

int parse_string(const std::string& str, Stack signs, Stack numbers) {
    int cur_index = 0;
    int out = str.size();
    for (int i = 0; i < out; i++) {
        if (str[i] == ' ')
            continue;
        else if (str[i]<= '9' && str[i] >= '0') {
            int val = str[i] - '0';
            i+=1;
            while (str[i] <= '9' && str[i] >= '0'){
                val = val * 10 + str[i] - '0';
                i ++;
            }
            numbers.push_in(val);
            i--; // for align
        }
        else {
            if (str[i] == '=') {
                while (!signs.is_empty()) {
                   calc_one(numbers, signs); 
                }
                return numbers.pop();
            } // end before ending.
            else if (str[i] == '+'
                    || str[i] == '-'
                    || str[i] == '*'
                    || str[i] == '/') {
                while (!signs.is_empty() && priority(signs.gtop()) >= priority(str[i])) {
                    calc_one(numbers, signs);
                }
                signs.push_in(str[i]);
            }
            else if (str[i] == '(') {
                signs.push_in(str[i]);
            }
            else if (str[i] == ')') {
                while (signs.gtop() != '(') {
                    calc_one(numbers, signs);
                }
                signs.pop();
            }
            else throw(std::runtime_error("not valid sign!"));
        }
    }
    while (!signs.is_empty()) {
        calc_one(numbers, signs);
    }
    return numbers.pop();
}

int main() {
    std::string expression;
    std::getline(std::cin, expression);
    Stack signs;
    Stack numbers;
    
    std::cout << parse_string(expression, signs, numbers) << std::endl;
}
