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
    for (size_t v = 0; v < 24; v++)
    {
        whimap::rwlock           lock;
        whimap::rwlock           lock2;
        std::barrier             lch1(2);
        std::barrier             lch2(14);
        std::barrier             lch3(2);
        std::barrier             lch4(14);
        std::vector<long>        a(512);
        std::vector<long>        b(512);
        std::mutex               mx2, mx1;
        std::vector<long>        r1, r2;
        std::vector<std::thread> init_writer;
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
                    std::this_thread::yield();
                }));

        for (size_t i = 0; i < 14; i++)
            reader.emplace_back(std::thread(
                [&]()
                {
                    lch2.arrive_and_wait();
                    std::lock_guard lg(lock);
                    long sum = std::accumulate(a.begin(), a.end(), 0);
                    std::lock_guard lmg(mx1);
                    r1.push_back(sum);
                    std::this_thread::yield();
                }));
        for (size_t i = 0; i < 2; i++)
            writer.emplace_back(std::thread(
                [&]()
                {
                    lch3.arrive_and_wait();
                    lock2.lock(whimap::rwlock::WRITE);
                    std::for_each(b.begin(), b.end(), [](long& v) { v = 20; });
                    lock2.unlock(whimap::rwlock::WRITE);
                    std::this_thread::yield();
                }));
        for (size_t i = 0; i < 14; i++)
            reader2.emplace_back(std::thread(
                [&]()
                {
                    lch4.arrive_and_wait();
                    std::lock_guard lg(lock2);
                    long sum = std::accumulate(b.begin(), b.end(), 0);
                    std::lock_guard lmg(mx2);
                    r2.push_back(sum);
                    std::this_thread::yield();
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
        std::lock_guard lmg1(mx2);
        std::lock_guard lmg2(mx1);
        std::cerr << v << ":" << std::endl;
        std::cerr << "R1:" << std::accumulate(r1.begin(), r1.end(), 0)
                  << std::endl;
        std::cerr << "R2:" << std::accumulate(r2.begin(), r2.end(), 0)
                  << std::endl;
        if (!((std::accumulate(r1.begin(), r1.end(), 0) == 71680) &&
              (std::accumulate(r2.begin(), r2.end(), 0) == 143360)))
        {
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
};
