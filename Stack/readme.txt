### Stack Implementations in C++

This project contains two implementations of the **Stack** abstract data type in C++, along with a comprehensive test suite to validate their functionality.

### Overview
The project includes:
-A Stack interface that defines the core stack operations (push, pop, top, etc.).
-Two implementations:
    #ArrayStack: Uses a dynamic array with automatic resizing.
    #ListStack: Uses a singly linked list for dynamic memory allocation.
Both implementations support the following operations:
    - `push(int)`
    - `pop()`
    - `top()`
    - `size()`
    - `empty()`
    - `clear()`
    - `print()`
-A test suite (test.cpp) that runs a series of tests to check the correctness of both implementations. It checks basic operations (push, pop, top), edge cases (empty stack behavior), and specific features like array resizing and stress testing with many operations.

### Files included:
├── arraystack.cpp - Array-based stack implementation
├── liststack.cpp - Linked list-based stack implementation
├── stack.h - Stack interface and shared definitions
├── test.cpp - Test framework and all test cases
├── readme.txt - This file, explaining the project and how to use it.

### How to compile and run
To compile the project, run the following command in the directory containing the source files:

        g++ -std=c++11 arraystack.cpp liststack.cpp test.cpp -o stack_test

This will generate an executable named stack_test. Run the executable with:

        ./stack_test

All the files should be in the same directory while compiling.