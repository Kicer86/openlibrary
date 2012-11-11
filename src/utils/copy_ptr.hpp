
#ifndef OPENLIBRARY_UTILS_COPY_PTR
#define OPENLIBRARY_UTILS_COPY_PTR


//deleter
namespace
{
    template<class T>
    void deleter(T *ptr)
    {
        delete ptr;
    }
}


//copy pointer
template<class T>
class copy_ptr
{
    public:
        typedef void (*Deleter)(T *);
        
        copy_ptr(T *v = nullptr, Deleter d = &deleter): m_ptr(v), m_deleter(d)
        {

        }

        copy_ptr(const T &other) 
        {
            delete m_ptr;
            m_ptr = new T(other);
        }

        virtual ~copy_ptr()
        {
            getDeleter()(m_ptr);
        }

        copy_ptr& operator=(const T &other)
        {
            *m_ptr = *other;
        }

        T* operator->()
        {
            return m_ptr;
        }

        const T* operator->() const
        {
            return m_ptr;
        }

        T& operator *()
        {
            return *m_ptr;
        }

        const T& operator *() const
        {
            return *m_ptr;
        }

    private:
        T *m_ptr;
        Deleter m_deleter;
        
        Deleter getDeleter() const
        {
            return m_deleter;
        }
};

#endif
