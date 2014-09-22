
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

//! Thread safe queue

/*!
 * TS_Queue is a thread safe queue based on std::deque.
 * It differs however from default containers: TS_Queue is meant to be used as a pipe. 
 * There is one side where one or many threads can safely write and there is other side from where one 
 * or many threads can read.
 * 
 * Use TS_Queue::push_back() for writing objects, and TS_Queue::pop_front() for reading objects.
 * TS_Queue has it maximum capacity defined on construction. When TS_Queue is full any attempt of pushing data will cause in wait operation.
 * 
 * Thread which destroys TS_Queue will be suspended until TS_Queue becomes empty.
 * 
 * based on: http://en.wikipedia.org/wiki/Producer-consumer_problem
 */

template<typename T>
class TS_Queue
{
    public:
        //! Constructor.
        //! @arg max_size maximum size of queue. When TS_Queue exceeds defined size, any write will cause writting thread to wait
        TS_Queue(size_t max_size):
            m_queue(),
            m_is_not_full(),
            m_is_not_empty(),
            m_queue_mutex(),
            m_max_size(max_size),
            m_stopped(false)
        {

        }

        //! Destructor.
        /*! Thread destroing TS_Queue will be suspended until TS_Queue becomes empty. */
        virtual ~TS_Queue()
        {
            stop();

            std::unique_lock<std::mutex> lock(m_queue_mutex);
            assert(m_queue.empty());
        }

        //! Write data to TS_Queue.
        /*! When queue is full, current thread will be suspended until some data is consumed by reader(s). */
        void push_back(const T &item)
        {
            enable_work();

            std::unique_lock<std::mutex> lock(m_queue_mutex);

            m_is_not_full.wait(lock, [&] { return m_queue.size() < m_max_size; } );  //wait for conditional_variable if there is no place in queue
            m_queue.push_back(item);
            m_is_not_empty.notify_all();
        }

        //! Get data.
        /*! When there is no data in queue, current thread will wait until data appear.
         * Returned type is Optional which can be empty in one situation: 
         * when thread was waiting for data and TS_Queue::stop() or TS_Queue's destructor were called.
         */        
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

        //! Take objects count.
        size_t size() const
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            const size_t result = m_queue.size();
            return result;
        }

        //! Check if TS_Queue is empty
        bool empty() const
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            const bool result = m_queue.empty();
            return result;
        }

        //! Release all threads waiting in TS_Queue::pop()
        void stop()
        {
            m_stopped = true;
            m_is_not_empty.notify_all();
        }

        //! Wait until data is available.
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
        std::atomic<bool> m_stopped;

        void wait_for_data(std::unique_lock<std::mutex>& lock)
        {
            auto precond = [&]
            {
                const bool condition = m_stopped == false && m_queue.empty();
                return !condition;
            };

            m_is_not_empty.wait(lock, precond);
        }

        void enable_work()
        {
            m_stopped = false;
        }
};

#endif
