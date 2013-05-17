
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

#include <condition_variable>
#include <mutex>

//based on: http://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem
template<class Queue>
class TS_Queue
{
    public:
        TS_Queue(size_t max_size): m_queue(), m_is_not_full(), m_is_not_empty(), m_mutex(), m_size(max_size) {}
        virtual ~TS_Queue() {}
        
        typedef typename Queue::value_type PopType;
        
        //writting
        template<class T>
        void push_back(const T &item)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
    
            m_is_not_full.wait(lock, [&] { return m_queue.size() < m_size; } );  //wait for conditional_variable if there is no place in queue
            m_queue.push_back(item);
    
            m_is_not_empty.notify_all();
        }
        
        //reading        
        PopType pop_front()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
             
            m_is_not_empty.wait(lock, [&] { return m_queue.empty() == false; });   //wait for signal if there is no data 
            
            PopType item = *(m_queue.begin());
            m_queue.pop_front();
                    
            m_is_not_full.notify_all();
            
            return item;
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
        
    private:
        Queue m_queue;
        std::condition_variable m_is_not_full;
        std::condition_variable m_is_not_empty;
        mutable std::mutex m_mutex;
        size_t m_size;
};

#endif
