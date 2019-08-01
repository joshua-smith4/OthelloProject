#include <iostream>
#include "cl.hpp"


int main()
{
    auto a = A<4>();
    std::cout << a.getInt() << "\n";
    return 0;

}
