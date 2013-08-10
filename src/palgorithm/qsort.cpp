
#include <assert.h>

#include <algorithm>
#include <iostream>
#include <initializer_list>
#include <vector>
#include <set>

#include <omp.h>
#include <sys/time.h>


template<class T>
inline void sort_swap(T *array, size_t i1, size_t i2)
{
    if (array[i1] > array[i2])
        std::swap(array[i1], array[i2]);
}

//bose-nelson algorithm based on:
// http://www.drdobbs.com/sorting-networks/184402663?pgno=2

template<class T>
class BoseNelsonSortingNetwork
{
    public:
        static constexpr size_t max_items = 32;
        
        BoseNelsonSortingNetwork(T *array): m_array(array) {}
        
        //sort array provided in constructor which is of size 'size'
        template<size_t size>
        inline void sort() const
        {
            bose<size>();
        }

    private:
        T *m_array;
        
        template<size_t i, size_t j>
        inline void P() const
        {
            //printf("swap(%lu, %lu);\n", i, j);
            sort_swap<T>(m_array, i, j);
        }

        template<size_t i, size_t x, size_t j, size_t y>
        inline void Pbracket() const
//int i;  /* value of first element in sequence 1 */
//int x;  /* length of sequence 1 */
//int j;  /* value of first element in sequence 2 */
//int y;  /* length of sequence 2 */
        {
            if(x == 1 && y == 1)
                P<i, j>(); /* 1 comparison sorts 2 items */
            else if(x == 1 && y == 2)
            {
                /* 2 comparisons inserts an item into an
                 * already sorted sequence of length 2. */
                P<i, (j + 1)>();
                P<i, j>();
            }
            else if(x == 2 && y == 1)
            {
                /* As above, but inserting j */
                P<i, j>();
                P<(i + 1), j>();
            }
            else
            {
                /* Recurse on shorter sequences, attempting
                 * to make the length of one subsequence odd
                 * and the length of the other even. If we
                 * can do this, we eventually merge the two. */
                constexpr size_t a = x / 2;
                constexpr size_t b = (x & 1) ? (y / 2) : ((y + 1) / 2);
                Pbracket<i, a, j, b>();
                Pbracket<(i + a), (x - a), (j + b), (y - b)>();
                Pbracket<(i + a), (x - a), j, b>();
            }
        }

        template<size_t i, size_t m>
        inline void Pstar() const
//int i;  /* value of first element in sequence */
//int m;  /* length of sequence */
        {
            if(m > 1)
            {
                /* Partition into 2 shorter sequences,
                 * generate a sorting method for each,
                 * and merge the two sub-networks. */
                constexpr size_t a = m / 2;
                Pstar<i, a>();
                Pstar<(i + a), (m - a)>();
                Pbracket<i, a, (i + a), (m - a)>();
            }
        }

        /* Calling bose(n) generates a network
         * to sort n items. See R. C. Bose & R. J. Nelson,
         * "A Sorting Problem", JACM Vol. 9, Pp. 282-296. */
        template<size_t n>
        inline void bose() const
        {
            //Pstar(1, n); /* sort the sequence {X1,...,Xn} */
            Pstar<0, n>(); //like in original but start from 0
        }
};


// sorting network:
// http://pages.ripco.net/~jgamble/nw.html
// http://en.wikipedia.org/wiki/Sorting_network
// parser: sed -e "s@\[\[@\[@g" -e "s@\]\]@\]@g" -e "s@\],\[@\]\[@g" -e "s@\[\([0-9]\+\),\([0-9]\+\)\]@sort_swap<T>(array, \1, \2);\n@g" input

template<class T>
class JumpTable
{
        typedef void (BoseNelsonSortingNetwork<T>::*SortingFunction)() const;
        
    public:
        JumpTable(): m_jumpTable(new SortingFunction[BoseNelsonSortingNetwork<T>::max_items + 1])
        {            
            Generator<T, BoseNelsonSortingNetwork<T>::max_items>().generate(m_jumpTable);
        }
        
        inline void call(BoseNelsonSortingNetwork<T> &boseNelson, size_t idx) const
        {
            assert(idx <= BoseNelsonSortingNetwork<T>::max_items);
            
            SortingFunction sort = m_jumpTable[idx];
            (boseNelson.*sort)();
        }
                
    private:
        SortingFunction *m_jumpTable;
        
        template<class P, int iteration>
        struct Generator
        {
            inline void generate(SortingFunction *table) const
            {
                Generator<P, iteration - 1>().generate(table);
                table[iteration] = &BoseNelsonSortingNetwork<T>::template sort<iteration>;          
            }
        };
        
        template<class P>
        struct Generator<P, -1>
        {
            inline void generate(SortingFunction *) const
            {
            }
        };

};


template<class T>
void fast_sort(T *array, size_t items)
{   
    static JumpTable<T> jmpTab;
        
    assert(items <= BoseNelsonSortingNetwork<T>::max_items || !"bad range");
    
    BoseNelsonSortingNetwork<T> boseNelson(array);
    jmpTab.call(boseNelson, items);
    
    assert(items == 0 || std::is_sorted(&array[0], &array[items]));
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


size_t pivotIdx(int *array, size_t size) __attribute__((noinline));
size_t pivotIdx(int *array, size_t size)
{
    std::pair<int, size_t> data[3] = 
    { 
      {array[0], 0},
      {array[size / 2], size / 2},
      {array[size - 1], size - 1}
    };
    
    fast_sort(data, 3);
    
    const size_t result = data[1].second;
    
    return result;
        
}

void quick_sort1(int *array, size_t size) __attribute__((noinline));
void quick_sort1(int *array, size_t size)
{
    if (size > BoseNelsonSortingNetwork<int>::max_items)
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
    else 
        fast_sort<int>(array, size);
}

void quick_sort(int *array, size_t size) __attribute__((noinline));
void quick_sort(int *array, size_t size)
{
    if (size > BoseNelsonSortingNetwork<int>::max_items)
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
    else
        fast_sort<int>(array, size);
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
    const int n = 5000000;

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
    //int table[17] = {1,3,2,9,8,0,4,7,5,6,10,11,12,13,16,14,15};
    //quick_sort(table, 17);
    //std::sort(&table[0], &table[16]);
    //BoseNelsonSortingNetwork<int> boseNelson(table);
    //boseNelson.sort<14>();
    
    //int table[14] = {1,3,2,9,8,0,4,7,5,6,10,11,12,13};
    //const int result = pivotIdx2(table, 14);
    //int table[9] = {1, 3, 5, 7, 9, 2, 4, 6, 8};
    //merge_sort(table, 9);

    test_algorithm(quick_sort, "pquick sort");
    test_algorithm(std_sort, "std::sort");
    //test_algorithm(bubble_sort, "bubble sort");
    test_algorithm(merge_sort, "merge sort");
    return 0;
}
