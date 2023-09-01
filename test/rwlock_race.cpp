#include <algorithm>
#include <whim_framework.hpp>
#include <utility>
#include <thread>
#include <numeric>
#include <iostream>
#include <barrier>
#include <vector>
int main()
{
    whimap::rwlock           lock;
    std::barrier             lch1(16);
    std::barrier             lch2(16);
    std::vector<long>        a(512);
    std::vector<std::thread> init_writer;
    std::mutex               mx2, mx1;
    std::vector<long>        r1, r2;
    std::vector<std::thread> reader;
    std::vector<std::thread> writer;
    std::vector<std::thread> reader2;
    // back to std::thread for AppleClang!
    for (size_t i = 0; i < 2; i++)
        init_writer.emplace_back(std::thread(
            [&]()
            {
                lch1.arrive_and_wait();
                lock.lock(whimap::rwlock::WRITE);
                std::for_each(a.begin(), a.end(), [](long& v) { v = 10; });
                lock.unlock(whimap::rwlock::WRITE);
            }));

    for (size_t i = 0; i < 14; i++)
        reader.emplace_back(std::thread(
            [&]()
            {
                lch1.arrive_and_wait();
                std::lock_guard lg(lock);
                long            sum = std::accumulate(a.begin(), a.end(), 0);
                std::lock_guard lmg(mx1);
                r1.push_back(sum);
            }));
    for (size_t i = 0; i < 2; i++)
        writer.emplace_back(std::thread(
            [&]()
            {
                lch2.arrive_and_wait();
                lock.lock(whimap::rwlock::WRITE);
                std::for_each(a.begin(), a.end(), [](long& v) { v = 20; });
                lock.unlock(whimap::rwlock::WRITE);
            }));
    for (size_t i = 0; i < 14; i++)
        reader2.emplace_back(std::thread(
            [&]()
            {
                lch2.arrive_and_wait();
                std::lock_guard lg(lock);
                long            sum = std::accumulate(a.begin(), a.end(), 0);
                std::lock_guard lmg(mx2);
                r2.push_back(sum);
            }));
    for (auto&& v : init_writer)
    {
        v.join();
    }
    for (auto&& v : reader)
    {
        v.join();
    }
    for (auto&& v : writer)
    {
        v.join();
    }
    for (auto&& v : reader2)
    {
        v.join();
    }
    std::cerr << "R1:" << std::accumulate(r1.begin(), r1.end(), 0) << std::endl;
    std::cerr << "R2:" << std::accumulate(r2.begin(), r2.end(), 0) << std::endl;
    if (!((std::accumulate(r1.begin(), r1.end(), 0) == 71680) &&
          (std::accumulate(r2.begin(), r2.end(), 0) == 143360)))
    {
        exit(EXIT_FAILURE);
    }
    return 0;
};
