
#ifndef OPENLIBRARY_PALGORITHM_TS_RESOURCE
#define OPENLIBRARY_PALGORITHM_TS_RESOURCE

#include <assert.h>

#include <mutex>
#include <memory>
#include <ostream>

#include "putils_export.h"

namespace ol
{

    //! Thread safe wrapper for objects.
    /*!
    ThreadSafeResource is a template class which allows to wrap any object (resource) into a thread safe container.
    Resource object must be constructed together with ThreadSafeResource (see ThreadSafeResource::ThreadSafeResource()).
    Then it can accessed by one thread at once. Any access to resource when it is already locked will cause in wait operation.
    To access locked resource call ThreadSafeResource::lock(). An Accessor object will be returned which is responsible for resource unlocking 
    when no longer used.
    */

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
            //! Notification interface
            struct INotify
            {
                virtual void unlocked() = 0;                       //!< Called when Accessor is being destructed and resource is unlocked.
            };

            //! ThreadSafeResource's helper class
            /*! Accessor is used by ThreadSafeResource for giving exclusive access to resource.
            *  Resource is normally being kept in ThreadSafeResource object.
            *  When ThreadSafeResource::lock() is called an Accessor object is returned with exclusive access to resource.
            *  As long as Accessor is alive, resource cannot be accessed by other threads.
            */
            struct Accessor
            {
                Accessor(std::mutex& mutex, T* object): m_lock(mutex), m_object(object), m_notify(nullptr) {}
                Accessor(Accessor&& other): m_lock(std::move(other.m_lock)),
                                            m_object(other.m_object),
                                            m_notify(other.m_notify)
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
                    m_notify = other.m_notify;

                    other.m_object = nullptr;
                    other.m_notify = nullptr;

                    return *this;
                }

                //! Destructor
                /*!
                * After destruction of Accessor resource is again available to other threads.
                * If INotify object was used with ThreadSafeResource::lock(INotify *), unlock() will be called on it
                */
                virtual ~Accessor()
                {
                    if (m_lock)
                        m_lock.unlock();

                    if (m_notify != nullptr)
                        m_notify->unlocked();
                }

                //! get reference to resource
                T& get()
                {
                    return *m_object;
                }

                //! get reference to const resource
                const T& get() const
                {
                    return *m_object;
                }

                //! get pointer to resource
                T* operator->()
                {
                    return m_object;
                }

                //! get pointer to const resource
                const T* operator->() const
                {
                    return m_object;
                }

                //! get reference to resource
                T& operator*()
                {
                    return *m_object;
                }

                //! get reference to const resource
                const T& operator*() const
                {
                    return *m_object;
                }

                private:
                    friend class ThreadSafeResource;
                    std::unique_lock<std::mutex> m_lock;
                    T* m_object;
                    INotify* m_notify;

                    void set(INotify* notify)
                    {
                        m_notify = notify;
                    }
            };

            friend struct Deleter;

            //! Contructor
            /*! Constructs ThreadSafeResource together with locked resource. */
            template<typename... Args>
            ThreadSafeResource(const Args&... args): m_mutex(), m_resource(args...)
            {
            }

            ThreadSafeResource(const ThreadSafeResource<T> &) = delete;
            ThreadSafeResource<T>& operator=(const ThreadSafeResource<T> &) = delete;

            //! Locks a resource and returns access to it.
            //! As long as returned Accessor object exists the resource will be locked and unavailable for other threads.
            Accessor lock()
            {
                Accessor accessor(m_mutex, &m_resource);

                return accessor;
            }

            //! Locks a resource and returns access to it.
            //! Behaves as lock() but calls INotify::unlocked() when object is being unlocked.
            Accessor lock(INotify* notify)
            {
                Accessor accessor = lock();
                accessor.set(notify);

                return accessor;
            }

        private:
            std::mutex m_mutex;
            T m_resource;
    };

    extern PUTILS_EXPORT ThreadSafeResource<std::ostream *> ThreadSafeOutput;

}

#endif
