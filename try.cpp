#include <iostream>

class A
{
    public:
        A(A const& a){ std::cout << "copy construct\n"; i = a.i; }
        A(A&& a){ i = a.i; std::cout << "moved construct\n"; }
        A& operator=(A&&){ std::cout << "move assign\n"; return *this; }
        A& operator=(A const&){ std::cout << "copy assign\n"; return *this; }
        int i;
        A(int i) : i(i) { std::cout << "normal construct\n"; }
};

int main()
{
    auto a = A(4);
    auto d = A(a);
    A c(A(3));
    A b(3);
    auto e = std::move(b);
    c = std::move(a);
    d = e;
    std::cout << a.i << " " << b.i << " " << c.i << " " << d.i << " " << e.i << "\n";
    return 0;
}
