#ifndef CS300HW_STACK_H
#define CS300HW_STACK_H
#include <vector>
#include <stdexcept> //for exceptions

template<typename T>
class Stack {
private:
    std::vector<T> data;
public:
    Stack();  // Constructor
    void push(const T& item);  // add elements to stack
    T pop();  // pop the top element in the stack and return it
    T& top();  // check the last element in the stack
    bool empty() const;  // check if the stack is empty or not
    void clear();  // clean the stack
};
#endif //CS300HW_STACK_H
