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
#include <type_traits>
#include <atomic>
namespace whimap
{
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
        std::mutex      metadata_lck;
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
                metadata_lck.lock();
                reader_count++;
                metadata_lck.unlock();
            }
            else if (me == WRITE)
            {
                metadata_lck.lock();
                if (reader_count != 0)
                {
                    metadata_lck.unlock();
                    while (reader_count != 0)
                    {
                        reader_count.wait(-1);
                    }
                    metadata_lck.lock();
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
                metadata_lck.lock();
                reader_count--;
                reader_count.notify_all();
                metadata_lck.unlock();
            }
            else if (me == WRITE)
            {
                metadata_lck.unlock();
            }
        }
    };
} // namespace whimap
