
//#define  _GLIBCXX_PARALLEL 

#include <assert.h>

#include <algorithm>
#include <iostream>
#include <initializer_list>

#include <sys/time.h>

#include "psort.hpp"

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

    RandomArray(const std::initializer_list<int> &i_list): m_array(nullptr), m_size(0)
    {
        m_size = i_list.size();
        m_array = new int[m_size];

        auto p = i_list.begin();
        for (int i = 0; i < m_size; i++, ++p)
            m_array[i] = *p;
    }
    
    RandomArray(const RandomArray &) = delete;

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
    
    void operator=(const RandomArray &) = delete;

    int *m_array;
    int m_size;
};


void test_algorithm(void (*sorting_function)(int *, int *), const char *name)
{
    const int n = 100000000;

    for (int m = 0; m < RandomArray::Max; m++)
    {
        RandomArray a(n, static_cast<RandomArray::Mode>(m));

        std::cout << "sorting array of " << n << " elements with '" << name << "' algorithm. Mode: " << m << std::endl;

        double start = getTime();
        sorting_function(&a.m_array[0], &a.m_array[n]);
        double end = getTime();

        std::cout << "sorting time: " << end-start << " sec" << std::endl;
    }
}


int main()
{
    int table[17] = {1,3,2,9,8,0,4,7,5,6,10,11,12,13,16,14,15};
    sort(&table[0], &table[17]);
    //std::sort(&table[0], &table[16]);
    //BoseNelsonSortingNetwork<int> boseNelson(table);
    //boseNelson.sort<14>();
    
    //int table[14] = {1,3,2,9,8,0,4,7,5,6,10,11,12,13};
    //const int result = pivotIdx2(table, 14);
    //int table[9] = {1, 3, 5, 7, 9, 2, 4, 6, 8};
    //merge_sort(table, 9);

    test_algorithm(sort, "pquick sort");
    test_algorithm(std::sort, "std::sort");
    //test_algorithm(bubble_sort, "bubble sort");
    //test_algorithm(merge_sort, "merge sort");
    return 0;
}
