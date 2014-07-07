
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

#include <assert.h>

#include <condition_variable>
#include <mutex>
#include <deque>

#include <boost/optional.hpp>

//based on: http://en.wikipedia.org/wiki/Producer-consumer_problem
template<typename T>
class TS_Queue
{
    public:
        typedef std::chrono::duration<std::chrono::milliseconds> TimeDuration;
        
        TS_Queue(size_t max_size): 
            m_queue(), 
            m_is_not_full(), 
            m_is_not_empty(), 
            m_mutex(), 
            m_size(max_size),
            m_threadsWaiting4Data(0) 
        {
            
        }
        
        virtual ~TS_Queue() 
        {
            break_popping();
        }

        //writting
        void push_back(const T &item)
        {
            push_back(item, Data::ItemType::Normal);
        }

        //reading
        boost::optional<T> pop_front()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            boost::optional<T> result;

            waitForEvent(lock);

            if (m_queue.empty() == false)
            {
                const Data item = *(m_queue.begin());

                if (item.type == Data::ItemType::Normal)
                {
                    result = item.data;
                    m_queue.pop_front();
                    m_is_not_full.notify_all();
                }
                else if (item.type == Data::ItemType::Empty)
                {
                    //do nothing - result is invalid, we just got info that we need to stop waiting
                    assert(m_queue.size() == 1); //this should be last and only item
                }
            }
            else
                assert(!"No data!");    //we were waiting for "is not empty" event, but queue is empty!

            return result;
        }

        size_t size() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            const size_t result = m_queue.size();
            return result;
        }

        bool empty() const
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            const bool result = m_queue.empty();
            return result;
        }
        
        //release all threads waiting in pop()
        void stop()
        {
            break_popping();
        }
        
        int threadsWaiting4Data() const
        {
            return m_threadsWaiting4Data;
        }

    private:
        struct Data
        {
            T data;
            enum class ItemType
            {
                Normal,
                Empty,
            } type;

            Data(const T& d, ItemType t): data(d), type(t) {}
        };

        std::deque<Data> m_queue;
        std::condition_variable m_is_not_full;
        std::condition_variable m_is_not_empty;
        mutable std::mutex m_mutex;
        size_t m_size;
        int m_threadsWaiting4Data;

        void push_back(const T &item, typename Data::ItemType type)
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            // Rise an assert if client tries to add item after empty item.
            // Empty item means, queue is going to be closed/destroyed.
            // See break_popping()
# ifndef NDEBUG
            if (m_queue.empty() == false)
            {
                const Data _item = m_queue.back();
                assert(_item.type != Data::ItemType::Empty);  // do not accept items when last item is empty
            }
# endif
            m_is_not_full.wait(lock, [&] { return m_queue.size() < m_size; } );  //wait for conditional_variable if there is no place in queue

            Data data(item, type);
            m_queue.push_back(data);

            m_is_not_empty.notify_all();
        }

        void waitForEvent(std::unique_lock<std::mutex>& lock)
        {
            ++m_threadsWaiting4Data;
            
            auto precond = [&]
            {
                return !m_queue.empty();
            };

            m_is_not_empty.wait(lock, precond);
            --m_threadsWaiting4Data;
        }
        
        // pushes an empty item just to stop waiting in pop_back.
        // Each pop_back since this function call will return invalid item.
        void break_popping()
        {
            push_back(T(), Data::ItemType::Empty);
            m_is_not_empty.notify_all();
        }
        
};

#endif
