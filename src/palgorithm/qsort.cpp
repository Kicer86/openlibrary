
#include <assert.h>

#include <algorithm>
#include <iostream>
#include <initializer_list>
#include <array>

#include <omp.h>
#include <sys/time.h>

/*****************************************************************************/
/*                                  PQSORT                                   */
/*****************************************************************************/

// left is the index of the leftmost element of the array
// right is the index of the rightmost element of the array (inclusive)
//   number of elements in subarray = right-left+1
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


size_t pivotIdx(int *array, size_t size)
{
    assert(size > 0);

    if (size == 1 || size == 2)
        return 0;

    if (size == 3)
    {
        //3 values: a, b and c
        if (array[0] > array[1])            //a > b?
            std::swap(array[0], array[1]);

        if (array[0] > array[2])
            std::swap(array[0], array[2]);

        if (array[1] > array[2])
            std::swap(array[1], array[2]);

        return 1;
    }

    if (size <= 5)
    {
        //find 3rd element
        for (int i = 0; i < 3; i++)
        {
            int minPos = i;
            int minVal = array[i];

            for (int j = i + 1; j < 5; j++)
            {
                if (array[j] < minVal)
                {
                    minVal = array[j];
                    minPos = j;
                }
            }

            std::swap(array[i], array[minPos]);
        }

        return 2;
    }

    std::vector<int> medians( ceil(size / 5.0) );
    std::vector<size_t> mediansOrigins(ceil(size / 5.0));

    for (size_t i = 0; i < size; i+=5)
    {
        const int subarray_size = size - i < 5? size -i : 5;  //5 or 1..4 for last part (if original size % 5 != 0)
        int *subarray = &array[i];
        size_t subarrayPivotIdx = pivotIdx(subarray, subarray_size);
        medians[i / 5] = subarray[subarrayPivotIdx];

        assert(subarrayPivotIdx + i >= 0);
        assert(subarrayPivotIdx + i < size);
        mediansOrigins[i / 5] = subarrayPivotIdx + i;
    }

    const size_t resultIdx = pivotIdx(medians.data(), medians.size());

    assert(resultIdx < mediansOrigins.size());

    return mediansOrigins[resultIdx];
}


void quick_sort1(int *array, size_t size)
{
    if (size > 1)
    {
        //std::cout << "partitioning array of size " << size << std::endl;
        const size_t pivot = pivotIdx(array, size);
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
}


void quick_sort(int *array, size_t size)
{
    if (size > 1)
    {
        //std::cout << "partitioning array of size " << size << std::endl;
        const size_t pivot = pivotIdx(array, size);
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

            case Random1:\
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
    const int n = 50000000;

    for (int m = 0; m < RandomArray::Max; m++)
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
    int table[11] = {1,3,2,9,8,0,4,7,5,6,10};
    const int result = pivotIdx(table, 11);

    test_algorithm(quick_sort, "pquick sort");
    test_algorithm(std_sort, "std::sort");
    //test_algorithm(bubble_sort, "bubble sort");
    return 0;
}
