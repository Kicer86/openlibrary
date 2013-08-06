
#include <assert.h>

#include <algorithm>
#include <iostream>
#include <initializer_list>
#include <vector>
#include <set>

#include <omp.h>
#include <sys/time.h>


template<class T, size_t items>
void fast_sort(T *array)
{
    static_assert(items < 4, "bad number of items");
    
    if (items == 0 || items == 1)
    {}
    else if (items == 2)
    {
        if (array[0] > array[1])
            std::swap(array[0], array[1]);
    }
    else if (items == 3)
    {
        if (array[0] > array[1])
            std::swap(array[0], array[1]);

        if (array[0] > array[2])
            std::swap(array[0], array[2]);

        if (array[1] > array[2])
            std::swap(array[1], array[2]);
    }
}


/*****************************************************************************/
/*                                  PQSORT                                   */
/*****************************************************************************/

// left is the index of the leftmost element of the array
// right is the index of the rightmost element of the array (inclusive)
//   number of elements in subarray = right-left+1
size_t partition(int *array, size_t left, size_t right, size_t pivotIndex) __attribute__((noinline));
size_t partition(int *array, size_t left, size_t right, size_t pivotIndex)
{    
    int pivotValue = array[pivotIndex];
    std::swap(array[pivotIndex], array[right]);  // Move pivot to end
    size_t storeIndex = left;
    for (size_t i = left; i < right; i++)  // left ≤ i < right
    {
        if (array[i] < pivotValue)
        {
            std::swap(array[i], array[storeIndex]);
            storeIndex++;
        }
    }
    std::swap(array[storeIndex], array[right]);  // Move pivot to its final place
    return storeIndex;
}


size_t pivotIdx2(int *array, size_t size) __attribute__((noinline));
size_t pivotIdx2(int *array, size_t size)
{
    struct Pair
    {
        Pair(): first(0), second(0) {}
        Pair(int f, int s): first(f), second(s) {}
        
        int first;
        int second;
        
        bool operator<(const Pair &other) const
        {
            return first < other.first;
        }
        
        bool operator>(const Pair &other) const
        {
            return first > other.first;
        }
        
        Pair& operator=(const Pair &other)
        {
            first = other.first;
            second= other.second;
            
            return *this;
        }
    };
        
    srand(size);

    Pair data[3];
    
    for (int i = 0; i < 3; i++)
    {
        const int idx = rand() % size;
        data[i] = Pair(array[idx], idx);
    }

    size_t result;
    
    if (data[0].first < data[1].first && data[1].first < data[2].first)
        result = data[1].second;
    else if (data[1].first < data[2].first && data[2].first < data[0].first)
        result = data[2].second;
    else
        result = data[0].second;
    
    return result;
        
}

void quick_sort1(int *array, size_t size) __attribute__((noinline));
void quick_sort1(int *array, size_t size)
{
    if (size > 3)
    {
        //std::cout << "partitioning array of size " << size << std::endl;
        const size_t pivot = pivotIdx2(array, size);
        size_t div = partition(array, 0, size - 1, pivot);

        if (div > 1)
        {
//            std::cout << "analyzing sub array of size " << div << " by thread #" << omp_get_thread_num() << std::endl;
            quick_sort1(array, div);
        }

        if (div < (size - 2) )
        {
//             std::cout << "analyzing sub array of size " << size - div << " by thread #" << omp_get_thread_num() << std::endl;
            quick_sort1(&array[div + 1], size - div - 1);
        }
    }
    else if (size == 3)
        fast_sort<int, 3>(array);
    else if (size == 2)
        fast_sort<int, 2>(array);
}

void quick_sort(int *array, size_t size) __attribute__((noinline));
void quick_sort(int *array, size_t size)
{
    if (size > 3)
    {
        //std::cout << "partitioning array of size " << size << std::endl;
        const size_t pivot = pivotIdx2(array, size);
        size_t div = partition(array, 0, size - 1, pivot);

        #pragma omp parallel sections default(shared)
        {
            #pragma omp section
            {
                if (div > 1)
                {
//                     std::cout << "analyzing sub array of size " << div << " by thread #" << omp_get_thread_num() << std::endl;
                    quick_sort1(array, div);
                }
            }

            #pragma omp section
            {
                if (div < (size - 2) )
                {
//                     std::cout << "analyzing sub array of size " << size - div << " by thread #" << omp_get_thread_num() << std::endl;
                    quick_sort1(&array[div + 1], size - div - 1);
                }
            }
        }
    }
    else if (size == 3)
        fast_sort<int, 3>(array);
    else if (size == 2)
        fast_sort<int, 2>(array);

}


void merge_sort(int *array, size_t size) __attribute__((noinline));
void merge_sort(int *array, size_t size)
{
    assert(size > 0);
    if (size == 2)
    {
        if (array[0] > array[1])
            std::swap(array[0], array[1]);
    }
    else if (size > 2)
    {
        const int middle = size/2;
        
        const size_t size_1 = middle;
        const size_t size_2 = size - middle;
        int *array_1 = array;
        int *array_2 = array + middle;
        
        //divide into two parts
        
        merge_sort(array, middle);
        merge_sort(array + middle, size - middle);
        
        //merge them
        size_t idx_1 = 0, idx_2 = 0;
        size_t inserter = 0;
        
        int *tmp = new int[size];

        while (idx_1 < size_1 || idx_2 < size_2)
        {
            bool use1 = idx_2 == size_2 || (idx_1 < size_1 && array_1[idx_1] < array_2[idx_2]);
                        
            if (use1)
                tmp[inserter++] = array_1[idx_1++];
            else
                tmp[inserter++] = array_2[idx_2++];
        }
        
        for (size_t i = 0; i < size; i++)
            array[i] = tmp[i];
        
        delete [] tmp;
    }    
}


/*****************************************************************************/


void bubble_sort(int array[], size_t size)
{
    int touchedIndex = size - 1;

    for (int i = 0; touchedIndex > 0; i++)
    {
        int tmpTouchedIndex = 0;
        for (int j = 0; j < touchedIndex; j++)
            if (array[j] > array[j + 1])
            {
                std::swap(array[j], array[j + 1]);
                tmpTouchedIndex = j;
            }

        touchedIndex = tmpTouchedIndex;
    }
}


double getTime()
{
    timeval tv;
    gettimeofday (&tv, NULL);
    return tv.tv_sec + 0.000001 * tv.tv_usec;
}

struct RandomArray
{
    enum Mode
    {
        Reverse,
        Random1,
        Random2,
        Random3,
        Max
    };

    RandomArray(int n, Mode mode): m_array(0), m_size(n)
    {
        m_array = new int[n];

        switch(mode)
        {
            case Reverse:
                for (int i = 0; i < n; i++)
                    m_array[i] = n - i;
                break;

            case Random1:
            case Random2:
            case Random3:
                srand(mode * 123);
                for (int i = 0; i < n; i++)
                    m_array[i] = rand();
                break;

            case Max:
                break;
        }
    }

    RandomArray(const std::initializer_list<int> &i_list)
    {
        m_size = i_list.size();
        m_array = new int[m_size];

        auto p = i_list.begin();
        for (int i = 0; i < m_size; i++, ++p)
            m_array[i] = *p;
    }

    ~RandomArray()
    {
        bool ok = true;
        for (int i = 0; i < m_size - 1; i++)
        {
            if (m_array[i] > m_array[i + 1])
                ok = false;
        }

        if (ok == false)
            std::cerr << "blad !!!" << std::endl;

        delete m_array;
    }

    int *m_array;
    int m_size;
};

void std_sort(int *array, size_t size)
{
    std::sort(&array[0], &array[size]);
}


void test_algorithm(void (*sorting_function)(int *array, size_t size), const char *name)
{
    const int n = 2000000;

    for (int m = 0; m < 1 /*RandomArray::Max*/; m++)
    {
        RandomArray a(n, (RandomArray::Mode)m);

        std::cout << "sorting array of " << n << " elements with '" << name << "' algorithm. Mode: " << m << std::endl;

        double start = getTime();
        sorting_function(a.m_array, n);
        double end = getTime();

        std::cout << "sorting time: " << end-start << " sec" << std::endl;
    }
}


int main()
{
    //int table[14] = {1,3,2,9,8,0,4,7,5,6,10,11,12,13};
    //const int result = pivotIdx2(table, 14);
    //int table[9] = {1, 3, 5, 7, 9, 2, 4, 6, 8};
    //merge_sort(table, 9);

    test_algorithm(quick_sort, "pquick sort");
    //test_algorithm(std_sort, "std::sort");
    //test_algorithm(bubble_sort, "bubble sort");
    //test_algorithm(merge_sort, "merge sort");
    return 0;
}
