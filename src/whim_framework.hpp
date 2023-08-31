// This file a part of WhimAP
// Copyright (C) 2023 WhimAP Authors

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#pragma once
#include <concepts>
#include <mutex>
#include <semaphore>
#include <atomic>
#include <type_traits>
namespace whimap
{
/**
 * @brief ailgnas a cacheline
 */
#define align_cache alignas(32)
/**
 * @brief ailgnas the memory alignment required by the simd
 * @tparam type of value
 */
#define align_simd(T)                              \
    alignas(std::experimental::memory_alignment_v< \
            std::experimental::native_simd<T>>)
 /** @brief A lock allows parallel reading, exclusive writing
   *
   * A rwlock maintains a count to allow multiply readers to enter
   * the critical section, and only allow one writer to enter it when
   * no one is reading.
   *
   * @attention assume you a reader on default to reuse std::lock_guard,
   * if not, call lock and unlock manually
   */
    class rwlock
    {
    private:
        std::atomic_int reader_count = 0;

    public:
        typedef enum
        {
            WRITE,
            READ
        } character_enum;
        /**
         * @brief lock a rwlock
         * 
         * @param me your role
         * @attention assume you a reader on default to reuse std::lock_guard,
         * if not, call lock and unlock manually
         */
        void lock(character_enum me = READ)
        {
            if (me == READ)
            {
                int i = -1;
                while (reader_count.compare_exchange_strong(i, -1))
                {
                    reader_count.wait(i);
                }
                reader_count++;
            }
            else if (me == WRITE)
            {
                int i = 0;
                while (!reader_count.compare_exchange_strong(i, -1))
                {
                    reader_count.wait(i);
                    i = 0;
                }
            }
        }
        /**
         * @brief unlock a rwlock
         * 
         * @param me your role
         * @attention assume you a reader on default to reuse std::lock_guard,
         * if not, call lock and unlock manually
         */
        void unlock(character_enum me = READ)
        {
            if (me == READ)
            {
                reader_count--;
                if (!reader_count)
                    reader_count.notify_one();
            }
            else if (me == WRITE)
            {
                reader_count++;
                reader_count.notify_all();
            }
        }
    };
} // namespace whimap
