
#ifndef OPENLIBRARY_PALGORITHM_TS_RESOURCE
#define OPENLIBRARY_PALGORITHM_TS_RESOURCE

#include <assert.h>

#include <mutex>
#include <memory>
#include <ostream>

#include "palgorithm_export.h"

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
        struct Accessor
        {
            Accessor(std::mutex& mutex, T* object): m_lock(mutex), m_object(object) {}
            Accessor(Accessor&& other): m_lock(std::move(other.m_lock)),
                                        m_object(other.m_object)
            {
                other.m_object = nullptr;
                other.m_notify = nullptr;
            }

            Accessor(const Accessor &) = delete;
            Accessor& operator=(const Accessor &) = delete;
            Accessor& operator=(Accessor&& other)
            {
                m_lock = std::move(other.m_lock);
                m_object = other.m_object;

                other.m_object = nullptr;
                other.m_notify = nullptr;

                return *this;
            }

            virtual ~Accessor()
            {
            }

            T& get()
            {
                return *m_object;
            }

            const T& get() const
            {
                return *m_object;
            }

            T* operator->()
            {
                return m_object;
            }

            const T* operator->() const
            {
                return m_object;
            }

            T& operator*()
            {
                return *m_object;
            }

            const T& operator*() const
            {
                return *m_object;
            }

            private:
                std::unique_lock<std::mutex> m_lock;
                T* m_object;
        };

        friend struct Deleter;

        template<typename... Args>
        ThreadSafeResource(const Args&... args): m_mutex(), m_resource(args...)
        {
        }

        ThreadSafeResource(const ThreadSafeResource<T> &) = delete;
        ThreadSafeResource<T>& operator=(const ThreadSafeResource<T> &) = delete;

        Accessor lock()
        {
            Accessor accessor(m_mutex, &m_resource);

            return accessor;
        }

    private:
        std::mutex m_mutex;
        T m_resource;
};

extern PALGORITHM_EXPORT ThreadSafeResource<std::ostream *> ThreadSafeOutput;

#endif
