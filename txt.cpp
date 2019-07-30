#include <iostream>

class A
{
public:
    struct B
    {
        friend class A;
    private:
        B(){std::cout << "B has been made\n";}
    public:
        B(int i) {std::cout << "int: " << i << "\n";}
    };
    void makeB(){B();}
};

int main()
{
    auto a = A();
    auto b = A::B(3);
    a.makeB();
    return 0;
}
