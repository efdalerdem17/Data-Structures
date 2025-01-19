#include "stack.h"

// empty constructor
template<typename T>
Stack<T>::Stack() {}

//push implementation
template<typename T>
void Stack<T>::push(const T& item) {
    data.push_back(item);
}
//pop implementation
template<typename T>
T Stack<T>::pop() {
    if (empty()) {
        throw std::runtime_error("Stack is empty");
    }
    T top = data.back();
    data.pop_back();
    return top;
}
//top implementation
template<typename T>
T& Stack<T>::top() {
    if (empty()) {
        throw std::runtime_error("Stack is empty");
    }
    return data.back();
}
//empty implementation
template<typename T>
bool Stack<T>::empty() const {
    return data.empty();
}
// clear implementation
template<typename T>
void Stack<T>::clear() {
    data.clear();
}
