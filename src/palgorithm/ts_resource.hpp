
#ifndef OPENLIBRARY_PALGORITHM_TS_RESOURCE
#define OPENLIBRARY_PALGORITHM_TS_RESOURCE

#include <assert.h>

#include <mutex>
#include <memory>
#include <ostream>

template<typename T>
class ThreadSafeResource
{
        struct Deleter
        {
            Deleter(std::unique_lock<std::mutex>* lock): m_lock(lock) {}
            Deleter(Deleter&& other): m_lock(nullptr)
            {
                m_lock = other.m_lock;
                other.m_lock = nullptr;
            }

            Deleter(const Deleter &) = delete;
            Deleter& operator=(const Deleter &) = delete;
            Deleter& operator=(Deleter&& other)
            {
                m_lock = other.m_lock;
                other.m_lock = nullptr;

                return *this;
            }

            virtual ~Deleter()
            {
                assert(m_lock == nullptr);
            }

            void operator() (T *)
            {
                //do not delete resource - delete lock instead, it will lock resource inside of ThreadSafeResource again

                delete m_lock, m_lock = nullptr;     //mutex will be free again
            }

            std::unique_lock<std::mutex>* m_lock;
        };

    public:
        friend struct Deleter;

        template<typename... Args>
        ThreadSafeResource(Args&... args): m_mutex(), m_resource(args...)
        {
        }

        ThreadSafeResource(const ThreadSafeResource<T> &) = delete;
        ThreadSafeResource<T>& operator=(const ThreadSafeResource<T> &) = delete;

        std::unique_ptr<T, Deleter> get()
        {
            std::unique_lock<std::mutex>* lock = new std::unique_lock<std::mutex>(m_mutex);    //lock on mutex
            std::unique_ptr<T, Deleter> result(&m_resource, Deleter(lock));

            return result;             //return resource, but don't release mutex - it will be released when Deleter is called
        }

    private:
        std::mutex m_mutex;
        T m_resource;
};

extern ThreadSafeResource<std::ostream &> ThreadSafeOutput;

#endif
