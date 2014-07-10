
/*
    Thread safe wrapper for queue-like containers
    Copyright (C) 2013  Michał Walenciak <Kicer86@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef OPENLIBRARY_PALGORITHM_TS_QUEUE
#define OPENLIBRARY_PALGORITHM_TS_QUEUE

#include <cassert>

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>

#include "utils/optional.hpp"

//based on: http://en.wikipedia.org/wiki/Producer-consumer_problem
template<typename T>
class TS_Queue
{
    public:
        TS_Queue(size_t max_size):
            m_queue(),
            m_is_not_full(),
            m_is_not_empty(),
            m_queue_mutex(),
            m_max_size(max_size),
            m_threadsWaiting4Data(0),
            m_stopped(false)
        {

        }

        virtual ~TS_Queue()
        {
            stop();

            std::unique_lock<std::mutex> lock(m_queue_mutex);
            assert(m_queue.empty());
        }

        //writting
        void push_back(const T &item)
        {
            enable_work();

            std::unique_lock<std::mutex> lock(m_queue_mutex);

            m_is_not_full.wait(lock, [&] { return m_queue.size() < m_max_size; } );  //wait for conditional_variable if there is no place in queue
            m_queue.push_back(item);
            m_is_not_empty.notify_all();
        }

        //reading
        Optional<T> pop_front()
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            Optional<T> result;

            wait_for_data(lock);

            if (m_queue.empty() == false)
            {
                result = *(m_queue.begin());
                m_queue.pop_front();
                m_is_not_full.notify_all();
            }

            return result;
        }

        size_t size() const
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            const size_t result = m_queue.size();
            return result;
        }

        bool empty() const
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            const bool result = m_queue.empty();
            return result;
        }

        //release all threads waiting in pop()
        void stop()
        {
            m_stopped = true;
            m_is_not_empty.notify_all();
        }

        int threadsWaiting4Data() const
        {
            return m_threadsWaiting4Data;
        }

        void waitForData()
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            Optional<T> result;

            wait_for_data(lock);
        }

    private:
        std::deque<T> m_queue;
        std::condition_variable m_is_not_full;
        std::condition_variable m_is_not_empty;
        mutable std::mutex m_queue_mutex;
        size_t m_max_size;
        std::atomic_int m_threadsWaiting4Data;
        std::atomic_bool m_stopped;

        void wait_for_data(std::unique_lock<std::mutex>& lock)
        {
            ++m_threadsWaiting4Data;

            auto precond = [&]
            {
                const bool condition = m_stopped == false && m_queue.empty();
                return !condition;
            };

            m_is_not_empty.wait(lock, precond);
            --m_threadsWaiting4Data;
        }

        void enable_work()
        {
            m_stopped = false;
        }
};

#endif
