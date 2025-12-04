#include <iostream>

class Base {
public:
    virtual void show() {
        std::cout << "Base class show function called." << std::endl;
    }
    virtual ~Base() {} // Virtual destructor for proper cleanup
};