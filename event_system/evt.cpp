#include "EventSystem.hpp"
#include <chrono>
#include <iostream>
#include <random>

std::default_random_engine gen;
std::uniform_int_distribution<int> distribution(0,2);

//std::atomic_flag g_mtx = ATOMIC_FLAG_INIT;
std::atomic_int counter = 0;
const int numIters = 10000;
void add(int i, int j) 
{ 
    for(auto k = 0; k < numIters; ++k) i+j;
    ++counter;
}
void sub(int i, int j)
{
    for(auto k = 0; k < numIters; ++k) i-j;
    ++counter;
}
void mult(int i, int j)
{
    for(auto k = 0; k < numIters; ++k) i*j;
    ++counter;
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
        for(auto i = 0; i < 1000000; ++i)
        {
            auto r = distribution(gen);
            a.post(evt::EventSystem<func_type>::Event(r, std::make_tuple(i-r,i)));
        }
        std::cout << "finished generating events\n";
        std::cout << "waiting for events to be handled\n";
    }
    auto end = steady_clock::now();
    auto duration = end - start;
    auto num_milli = duration_cast<milliseconds>(duration).count();
    std::cout << "time: " << num_milli << " milliseconds.\n"; 
    std::cout << "num events: " << counter << "\n";
    return 0;
}
