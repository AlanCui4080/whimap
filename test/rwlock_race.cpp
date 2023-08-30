#include "whim_framework.hpp"
#include <thread>
#include <numeric>
#include <iostream>
int main()
{   
    whimap::rwlock lock;
    std::array<long,512> a{0};
    std::array<std::jthread, 2> init_writer{std::jthread([&]() {
        lock.lock(whimap::rwlock::WRITE);
        std::for_each(a.begin(),a.end(),[](long& v){v=10;});
        lock.unlock(whimap::rwlock::WRITE);
    })};
    std::mutex mx;
    std::vector<long> r1,r2;
    std::array<std::jthread, 14> reader{std::jthread([&]() {
        std::lock_guard lg(lock);
        long sum = std::accumulate(a.begin(),a.end(),0);
        std::lock_guard mx(lock);
        r1.push_back(sum);
    })};
    std::array<std::jthread, 2> writer{std::jthread([&]() {
        lock.lock(whimap::rwlock::WRITE);
        std::for_each(a.begin(),a.end(),[](long& v){v=20;});
        lock.unlock(whimap::rwlock::WRITE);
    })};
    std::array<std::jthread, 14> reader2{std::jthread([&]() {
        std::lock_guard lg(lock);
        long sum = std::accumulate(a.begin(),a.end(),0);
        std::lock_guard mx(lock);
        r2.push_back(sum);
    })};
    return (std::accumulate(r1.begin(),r1.end(),0) == 5120)
    && (std::accumulate(r1.begin(),r1.end(),0) == 10240);
};
