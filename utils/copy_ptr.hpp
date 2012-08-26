
#ifndef UTILS_COPY_PTR
#define UTILS_COPY_PTR

template<class T>
class copy_ptr
{
    public:
        copy_ptr(T *v = nullptr): value(v)
        {

        }

        copy_ptr(const T &other)
        {
            delete value;
            value = new T();

            *value = *other;
        }

        virtual ~copy_ptr()
        {
            delete value;
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
};

#endif
