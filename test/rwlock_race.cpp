#include "whim_framework.hpp"
#include <thread>
#include <numeric>
#include <iostream>
#include <barrier>
int main()
{
    whimap::rwlock            lock;
    std::barrier              lch1(16);
    std::barrier              lch2(16);
    std::vector<long>         a(512);
    std::vector<std::jthread> init_writer;
    std::mutex                mx2, mx1;
    std::vector<long>         r1, r2;
    std::vector<std::jthread> reader;
    std::vector<std::jthread> writer;
    std::vector<std::jthread> reader2;
    for (size_t i = 0; i < 2; i++)
        init_writer.emplace_back(std::move(std::jthread(
            [&]()
            {
                lch1.arrive_and_wait();
                lock.lock(whimap::rwlock::WRITE);
                std::for_each(a.begin(), a.end(), [](long& v) { v = 10; });
                lock.unlock(whimap::rwlock::WRITE);
                exit(EXIT_SUCCESS);
            })));

    for (size_t i = 0; i < 14; i++)
        reader.emplace_back(std::move(std::jthread(
            [&]()
            {
                lch1.arrive_and_wait();
                std::lock_guard lg(lock);
                long            sum = std::accumulate(a.begin(), a.end(), 0);
                std::lock_guard lmg(mx1);
                r2.push_back(sum);
                exit(EXIT_SUCCESS);
            })));
    for (size_t i = 0; i < 2; i++)
        writer.emplace_back(std::move(std::jthread(
            [&]()
            {
                lch2.arrive_and_wait();
                lock.lock(whimap::rwlock::WRITE);
                std::for_each(a.begin(), a.end(), [](long& v) { v = 20; });
                lock.unlock(whimap::rwlock::WRITE);
                exit(EXIT_SUCCESS);
            })));
    for (size_t i = 0; i < 14; i++)
        reader2.emplace_back(std::move(std::jthread(
            [&]()
            {
                lch2.arrive_and_wait();
                std::lock_guard lg(lock);
                long            sum = std::accumulate(a.begin(), a.end(), 0);
                std::lock_guard lmg(mx2);
                r2.push_back(sum);
                exit(EXIT_SUCCESS);
            })));

    if (!((std::accumulate(r1.begin(), r1.end(), 0) == 5120) &&
        (std::accumulate(r2.begin(), r2.end(), 0) == 10240)))
    {
        exit(EXIT_FAILURE);
    }
    return 0;
};
