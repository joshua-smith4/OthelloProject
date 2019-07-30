#include "EventSystem.hpp"
#include <chrono>
#include <iostream>
#include <random>

std::default_random_engine gen;
std::uniform_int_distribution<int> distribution(0,2);

std::atomic_flag g_mtx = ATOMIC_FLAG_INIT;
int counter = 0;
const int numIters = 10000;
void add(int i, int j) 
{ 
    for(auto k = 0; k < numIters; ++k) i+j;
    while(g_mtx.test_and_set(std::memory_order_acquire));
    ++counter;
    g_mtx.clear(std::memory_order_release);
}
void sub(int i, int j)
{
    for(auto k = 0; k < numIters; ++k) i-j;
    while(g_mtx.test_and_set(std::memory_order_acquire));
    ++counter;
    g_mtx.clear(std::memory_order_release);
}
void mult(int i, int j)
{
    for(auto k = 0; k < numIters; ++k) i*j;
    while(g_mtx.test_and_set(std::memory_order_acquire));
    ++counter;
    g_mtx.clear(std::memory_order_release);
}

using func_type = void(int, int);
int main()
{
    using namespace std::chrono;
    auto start = steady_clock::now();
    {
        evt::EventSystem<func_type> a(8);
        a.add_handler(0, add);
        a.add_handler(1, sub);
        a.add_handler(2, mult);
        for(auto i = 0; i < 10000000; ++i)
        {
            auto r = distribution(gen);
            a.post(evt::EventSystem<func_type>::Event(r, std::make_tuple(i-r,i)));
        }
        std::cout << "finished generating events\n";
/*        a.wait();
        std::cout << "waiting for input\n";
        int j;
        std::cin >> j;
        for(auto i = 0; i < 10000000; ++i)
        {
            auto r = distribution(gen);
            a.post(evt::EventSystem<func_type>::Event(r, std::make_tuple(i-r,i)));
        }*/
    }
    auto end = steady_clock::now();
    auto duration = end - start;
    auto num_milli = duration_cast<milliseconds>(duration).count();
    std::cout << "time: " << num_milli << " milliseconds.\n"; 
    std::cout << "num events: " << counter << "\n";
    return 0;
}
