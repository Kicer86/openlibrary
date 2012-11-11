
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
        
        copy_ptr(T *v = nullptr, Deleter d = &deleter): value(v), m_deleter(d)
        {

        }

        copy_ptr(const T &other) 
        {
            delete value;
            value = new T(other);
        }

        virtual ~copy_ptr()
        {
            getDeleter()(value);
        }

        copy_ptr& operator=(const T &other)
        {
            *value = *other;
        }

        T* operator->()
        {
            return value;
        }

        const T* operator->() const
        {
            return value;
        }

        T& operator *()
        {
            return *value;
        }

        const T& operator *() const
        {
            return *value;
        }

    private:
        T *value;
        Deleter m_deleter;
        
        Deleter getDeleter() const
        {
            return m_deleter;
        }
};

#endif
