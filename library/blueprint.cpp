#include <iostream>

extern "C"
{
    void hello(void)
    {
        std::cout << "Hello, World!" << std::endl;
    }
}
