
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

    template<class T>
    void copier(T* &to_ptr, const T *from_ptr)
    {
        *to_ptr = *from_ptr;
    }
}


//data pointer
//data_ptr can be used for storing class' private data which on class copy should also be copied
//operator= of data_ptr and copy consturctor of data_ptr are calling operator= on encapsulated pointer
template<class T>
class data_ptr
{
    public:
        typedef void (*Deleter)(T *);
        typedef void (*Copier)(T* &, const T *);

        data_ptr(T *ptr = nullptr, Deleter d = &deleter, Copier c = &copier): m_ptr(ptr), m_deleter(d), m_copier(c)
        {

        }

        data_ptr(const data_ptr<T> &other)
        {
            copy(other);
        }

        virtual ~data_ptr()
        {
            del();
        }

        data_ptr& operator=(T *ptr)
        {
            del();
            m_ptr = ptr;
        }

        data_ptr& operator=(const data_ptr<T> &other)
        {
            del();
            copy(other);
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
        Copier m_copier;

        //getting operators
        Deleter getDeleter() const
        {
            return m_deleter;
        }

        Copier getCopier() const
        {
            return m_copier;
        }

        //copy another copy_ptr
        void copy(const data_ptr<T> &other)
        {
            //copy data
            copy(other.m_ptr);

            //copy operators
            m_deleter = other.m_deleter;
            m_copier = other.m_copier;
        }

        //copy object
        void copy(T *ptr)
        {
            getCopier()(m_ptr, ptr);
        }

        //delete object
        void del() const
        {
            getDeleter()(m_ptr);
        }

};

#endif
