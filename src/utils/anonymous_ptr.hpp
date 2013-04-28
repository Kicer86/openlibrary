
#ifndef OPENLIBRARY_UTILS_ANONYMOUS_PTR
#define OPENLIBRARY_UTILS_ANONYMOUS_PTR

/*  
 *  anonymous_ptr a class which behaves as std::shared_ptr but does not
 *  require T to be known type
 */

template<class T, class D>
class anonymous_ptr
{
    public:
        explicit anonymous_ptr(T *d, D *deleter) noexcept: m_data(nullptr), m_deleter(deleter), m_ptr(nullptr)
        {
            assign(d);
        }
        
        explicit anonymous_ptr(const anonymous_ptr<T, D> &other) noexcept: m_data(nullptr), m_deleter(nullptr), m_ptr(nullptr)
        {
            *this = other;
        }
        
        virtual ~anonymous_ptr()
        {
            release();
        }
        
        anonymous_ptr<T, D>& operator=(const anonymous_ptr<T, D> &other)
        {
            release();            
            assign(other.m_ptr, other.m_data);
            m_deleter = other.m_deleter;
            
            m_data->m_ref_count++;
            
            return *this;
        }
        
        void reset(T* d)
        {
            release();
            assign(d);
        }
        
    private:
        struct Data
        {
            Data(): m_ref_count(1) {}
            int m_ref_count; 
        } *m_data;
        
        void release()
        {
            if (m_data != nullptr)
            {
                m_data->m_ref_count--;
            
                //delete data only when last referer destroyed
                if (m_data->m_ref_count == 0)
                {                
                    m_deleter->deinit();
                    delete m_data, m_data = nullptr;
                    
                    m_ptr = nullptr;
                }
            }            
        }
        
        void assign(T *d, Data *data = nullptr)
        {
            m_ptr = d;
            
            if (data == nullptr)
                m_data = new Data;
            else
                m_data = data;
        }
        
        D *m_deleter;
        T *m_ptr;
};

#endif
