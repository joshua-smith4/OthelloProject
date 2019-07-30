#include <tuple>
#include <utility>
#include <iostream>
#include <functional>
#include <vector>


template <class T>
struct A;

template <class Ret, class... Args>
struct A<Ret(Args...)>
{
    using handler = std::function<Ret(Args...)>;
    std::vector<handler> funcs;
    std::vector<Ret> call(Args... args)
    {
        auto retVal = std::vector<Ret>();
        for(auto&& func : funcs)
        {
            retVal.push_back(std::invoke(func, std::forward<Args>(args)...));
        }
        return retVal;
    }
    void add_func(handler func)
    {
        funcs.push_back(func);
    }
};

int f1(int i, int j){ return i + j; }

struct f2
{
    int operator()(int i, int j) { return i - j; }
};

    template <class... Args>
    struct Event
    {
        Event(unsigned long i, std::tuple<Args...> a) : id(i), args(a) {}
        const unsigned id;
        const std::tuple<Args...> args; 
    };

template <class... Args>
Event<Args...> makeEvent1(std::tuple<Args...> a)
{
    return Event(1, a);
}

void printStuff(int a, int b) {std::cout << a << " " << b << "\n";}

    template <class... Args>
    class EventSystem
    {
        public:
        struct Event
        {
            Event(unsigned i, std::tuple<Args...> a) : id(i), args(a) {}
            const unsigned id;
            const std::tuple<Args...> args; 
        };
    };
int main()
{
    auto f3 = [](int i, int j) { return i * j; };
    auto func_container = A<int(int,int)>();
    func_container.add_func(f1);
    func_container.add_func(f2());
    func_container.add_func(f3);
    std::tuple<int,int> args({5,2});
    std::apply(printStuff, args);
    for(auto&& elem : func_container.call(5,2)) std::cout << elem << std::endl;

    using ES = EventSystem<int,char,double>;
    ES::Event e(4,std::make_tuple(4,'j',2.3));
    auto e1 = makeEvent1(std::make_tuple(2,4,'e',4.3));
    std::cout << e.id << "\n";
    std::cout << e1.id << "\n";
    return 0;
};
