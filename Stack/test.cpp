#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include "stack.h"

/**
 * A test case structure to hold test metadata and function
 */
struct TestCase {
    std::string name;
    std::function<bool(Stack*)> test_func;
    
    TestCase(const std::string& name, std::function<bool(Stack*)> func)
        : name(name), test_func(func) {}
};

/**
 * Tester class that manages test registration and execution
 */
class StackTester {
private:
    std::vector<TestCase> test_cases;
    std::vector<Stack*> stacks;
    int passed_tests = 0;
    int total_tests = 0;

public:
    /**
     * Add a stack implementation to be tested
     */
    void add_stack(Stack* stack) {
        stacks.push_back(stack);
    }
    
    /**
     * Register a test case
     */
    void register_test(const std::string& name, std::function<bool(Stack*)> test_func) {
        test_cases.push_back(TestCase(name, test_func));
    }
    
    /**
     * Get the implementation type name of a stack
     */
    static std::string get_stack_type(Stack* stack) {
        if (dynamic_cast<ArrayStack*>(stack)) {
            return "ArrayStack";
        } else if (dynamic_cast<ListStack*>(stack)) {
            return "ListStack";
        } else {
            return "Unknown Stack";
        }
    }
    
    /**
     * Run a single test on a specific stack implementation
     */
    bool run_test(const TestCase& test, Stack* stack) {
        std::string stack_type = get_stack_type(stack);
        std::cout << "  " << stack_type << " - " << test.name << ": ";
        
        bool result = test.test_func(stack);
        
        if (result) {
            std::cout << "\033[32mPASSED\033[0m" << std::endl; // Green text
        } else {
            std::cout << "\033[31mFAILED\033[0m" << std::endl; // Red text
        }
        
        return result;
    }
    
    /**
     * Run all registered tests on all registered stack implementations
     */
    bool run_all_tests() {
        // Print header
        std::cout << "===== STACK IMPLEMENTATION TESTS =====" << std::endl;
        
        total_tests = 0;
        passed_tests = 0;
        
        // For each stack implementation
        for (auto stack : stacks) {
            std::string stack_type = get_stack_type(stack);
            std::cout << "\n----- " << stack_type << " Tests -----" << std::endl;
            
            // Run each test case
            for (const auto& test : test_cases) {
                total_tests++;
                if (run_test(test, stack)) {
                    passed_tests++;
                }
            }
        }
        
        // Print test summary
        std::cout << "\n===== TEST SUMMARY =====" << std::endl;
        std::cout << "Total tests: " << total_tests << std::endl;
        std::cout << "Passed: " << passed_tests << std::endl;
        std::cout << "Failed: " << (total_tests - passed_tests) << std::endl;
        
        if (passed_tests == total_tests) {
            std::cout << "All tests PASSED!" << std::endl;
        } else {
            std::cout << "Some tests FAILED!" << std::endl;
        }
        
        return passed_tests == total_tests;
    }
    
    /**
     * Cleanup allocated resources
     */
    void cleanup() {
        for (auto stack : stacks) {
            delete stack;
        }
        stacks.clear();
    }
};

/**
 * Individual test functions for each stack operation
 */

// Test for push method
bool test_push(Stack* stack) {
    stack->clear();
    stack->push(42);
    
    return stack->size() == 1 && stack->top() == 42;
}

// Test for pop method
bool test_pop(Stack* stack) {
    stack->clear();
    stack->push(10);
    stack->push(20);
    
    int popped = stack->pop();
    
    return popped == 20 && stack->size() == 1 && stack->top() == 10;
}

// Test for top method
bool test_top(Stack* stack) {
    stack->clear();
    stack->push(30);
    stack->push(40);
    
    int top_value = stack->top();
    
    // Ensure top doesn't modify the stack
    return top_value == 40 && stack->size() == 2;
}

// TODO: Implement test for the size method
bool test_size(Stack* stack) {
    // TODO: Implement this test case
    stack->clear();
    stack->push(20);
    stack->push(10);
    stack->push(30);
    if (stack->size() != 3) return false;

    stack->pop();
    if (stack->size() != 2) return false;

    stack->clear();
    if (stack->size() != 0) return false;

    return true; // Placeholder
}

// TODO: Implement test for the empty method
bool test_empty(Stack* stack) {
    // TODO: Implement this test case
    stack->clear();

    if (!stack->empty()) return false;

    stack->push(10);
    stack->push(20);
    stack->push(30);
    if (stack->empty())
        return false;

    for(int i=0;i<3;i++)
    {
        stack->pop();
    }
    if (!stack->empty()) return false;

    return true; // Placeholder
}

// TODO: Implement test for the clear method
bool test_clear(Stack* stack) {
    // TODO: Implement this test case
    stack->clear();
    
    stack->push(1);
    stack->push(2);
    stack->push(3);

    stack->clear();
    int size = stack->size();
    if(size!=0)
        return false;

    return true; // Placeholder
}

// TODO: Implement test for multiple push/pop operations
bool test_multiple_push_pop(Stack* stack) {
    // TODO: Implement this test case to verify LIFO behavior with multiple elements
    stack->clear();
    for (int i = 1; i <= 5;i++)
    {
        stack->push(i);
    }
    for (int i = 5; i >= 1;i--)
    {
        if(stack->pop()!=i)
            return false;
    }
    if(!stack->empty())
        return false;
    return true; // Placeholder
}

// TODO: Implement test for edge cases (empty stack operations)
bool test_empty_stack_operations(Stack* stack) {
    // TODO: Test behavior when popping or checking top of an empty stack
    stack->clear();
    bool b = true;
    int p1 = stack->pop();
    b = b && (p1 == -1);

    int p2 = stack->top();
    b= b && (p2 == -1);

    return b && stack->empty() && stack->size() == 0;
}

// TODO: Implement test for ArrayStack resizing
bool test_array_resizing(Stack* stack) {
    // TODO: Test if ArrayStack properly resizes when capacity is reached
    stack->clear();
    for (int i = 1; i <= 10;i++)
    {
        stack->push(i);
    }
    int s1 = stack->size();
    for (int i = 1; i <= 10;i++)
    {
        stack->push(i);
    }
    int s2 = stack->size();
    if(s1!=10 || s2!=20)
        return false;
    return true; // Placeholder
}

// TODO: Implement test for large number of operations
bool test_stress(Stack* stack) {
    // TODO: Test stability with a large number of push and pop operations
    stack->clear();
    for (int i = 1; i <= 10000;i++)
    {
        stack->push(i);
    }
    int s = stack->size();
    if(s!=10000)
        return false;

    for (int i = 10000; i >= 1;i--)
    {
        int p=stack->pop();
        if(p!=i)
            return false;
    }
    if(!(stack->empty()) || stack->size()!=0)
        return false;
    return true; // Placeholder
}

/**
 * Main function
 */
int main() {
    // Create tester instance
    StackTester tester;
    
    // Register stack implementations
    tester.add_stack(new ArrayStack());
    tester.add_stack(new ListStack());
    
    // Register test cases with implemented tests
    tester.register_test("Push Operation", test_push);
    tester.register_test("Pop Operation", test_pop);
    tester.register_test("Top Operation", test_top);
    
    // Register test cases with TODO tests
    tester.register_test("Size Operation", test_size);
    tester.register_test("Empty Operation", test_empty);
    tester.register_test("Clear Operation", test_clear);
    tester.register_test("Multiple Push/Pop", test_multiple_push_pop);
    tester.register_test("Empty Stack Operations", test_empty_stack_operations);
    tester.register_test("Array Resizing", test_array_resizing);
    tester.register_test("Stress Test", test_stress);
    
    // Run all tests
    bool all_passed = tester.run_all_tests();
    
    // Clean up
    tester.cleanup();
    
    return all_passed ? 0 : 1;
}