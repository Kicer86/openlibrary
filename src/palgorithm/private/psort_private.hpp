
#ifndef PSORT_PRIVATE
#define PSORT_PRIVATE

#include <assert.h>
#include <omp.h>
#include <algorithm>
#include <type_traits>

namespace OpenLibrary
{
    namespace Private
    {

        template<class ArrayIterator>
        void sort_swap(ArrayIterator i1, ArrayIterator i2)
        {
            if (*i1 > *i2)
                std::swap(*i1, *i2);
        }

//bose-nelson algorithm based on:
// http://www.drdobbs.com/sorting-networks/184402663?pgno=2

        template<class ArrayIterator>
        class BoseNelsonSortingNetwork
        {
            public:
                static constexpr int max_items = 32;

                BoseNelsonSortingNetwork(ArrayIterator arrayBegin): m_array(arrayBegin) {}

                //sort array provided in constructor which is of size 'size'
                template<size_t size>
                void sort() const
                {
                    bose<size>();
                }

            private:
                ArrayIterator m_array;

                template<size_t i, size_t j>
                void P() const
                {
                    //printf("swap(%lu, %lu);\n", i, j);
                    sort_swap(m_array + i, m_array + j);
                }

                template<size_t i, size_t x, size_t j, size_t y>
                void Pbracket() const
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
                        P < i, (j + 1) > ();
                        P<i, j>();
                    }
                    else if(x == 2 && y == 1)
                    {
                        /* As above, but inserting j */
                        P<i, j>();
                        P < (i + 1), j > ();
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
                        Pbracket < (i + a), (x - a), (j + b), (y - b) > ();
                        Pbracket < (i + a), (x - a), j, b > ();
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
                        Pstar < (i + a), (m - a) > ();
                        Pbracket < i, a, (i + a), (m - a) > ();
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

                void call(BoseNelsonSortingNetwork<T> &boseNelson, int idx) const
                {
                    assert(idx <= BoseNelsonSortingNetwork<T>::max_items);

                    SortingFunction sort = m_jumpTable[idx];
                    (boseNelson.*sort)();
                }

            private:
                SortingFunction* m_jumpTable;

                template<class P, int iteration>
                struct Generator
                {
                    inline void generate(SortingFunction* table) const
                    {
                        Generator < P, iteration - 1 > ().generate(table);
                        table[iteration] = &BoseNelsonSortingNetwork<T>::template sort<iteration>;
                    }
                };

                template<class P>
                struct Generator < P, -1 >
                {
                    inline void generate(SortingFunction*) const
                    {
                    }
                };

        };


        template<class ArrayIterator>
        void fast_sort(ArrayIterator left, ArrayIterator right)
        {
            const auto items = right - left;
            static JumpTable<ArrayIterator> jmpTab;

            assert(items <= BoseNelsonSortingNetwork<ArrayIterator>::max_items || !"bad range");

            BoseNelsonSortingNetwork<ArrayIterator> boseNelson(left);
            jmpTab.call(boseNelson, items);

            assert(items == 0 || std::is_sorted(left, right));
        }


        /*****************************************************************************/
        /*                                  PQSORT                                   */
        /*****************************************************************************/

// http://pl.wikipedia.org/wiki/Sortowanie_introspektywne

// left is the index of the leftmost element of the array
// right is the index of the rightmost element of the array (inclusive)
//   number of elements in subarray = right-left+1

        template<class ArrayIterator>
        ArrayIterator partition(ArrayIterator left, ArrayIterator right, ArrayIterator pivotIndex)
        {
            int pivotValue = *pivotIndex;
            const ArrayIterator last_item = right - 1;
            std::swap(*pivotIndex, *last_item);  // Move pivot to end
            ArrayIterator storeIndex = left;

            for (ArrayIterator i = left; i < right; ++i)  // left ≤ i < right
            {
                if (*i < pivotValue)
                {
                    std::swap(*i, *storeIndex);
                    storeIndex++;
                }
            }

            std::swap(*storeIndex, *last_item);  // Move pivot to its final place
            return storeIndex;
        }

        
        template<class ArrayIterator>
        ArrayIterator pivotIdx(ArrayIterator left, ArrayIterator right)
        {
            typedef decltype (*left) ArrayType;
            --right;
            
            ArrayIterator mid = left + (right - left) / 2;
            std::pair<ArrayType, ArrayIterator> data[3] =
            {
                {*left, left},
                {*mid, mid},
                {*right, right}
            };

            fast_sort(&data[0], &data[2]);

            const ArrayIterator result = data[1].second;

            return result;

        }

        
        template<class ArrayIterator>
        void quick_sort(ArrayIterator left, ArrayIterator right, int avail_cpus)
        {
            assert(avail_cpus > 0);
            const auto size = right - left;
            
            if (size > BoseNelsonSortingNetwork<ArrayIterator>::max_items)
            {
                //std::cout << "partitioning array of size " << size << std::endl;
                ArrayIterator pivot = pivotIdx(left, right);                                
                ArrayIterator div = partition(left, right, pivot);
                
                const auto div_pos = div - left;
                
                const int cores_to_use = avail_cpus >= 2? 2: 1;   // 2 - parallel; 1 - single
                const int cores_for_sub = avail_cpus / cores_to_use;

                #pragma omp parallel sections default(shared) num_threads(cores_to_use)
                {                    
                    #pragma omp section
                    {
                        if (div_pos > 1)
                            quick_sort(left, div, cores_for_sub);                        
                        
                    }

                    #pragma omp section
                    {
                        if (div_pos < (size - 2) )
                            quick_sort(div + 1, right, cores_for_sub);                        
                    }
                }
                 
            }
            else
                fast_sort(left, right);
        }
        
        
        template<class ArrayIterator>
        struct MergeSortData
        {
            ArrayIterator main_left;
            ArrayIterator tmp_left;
            
            long size;
            
            ArrayIterator getWork(int level)
            {
                return level % 2 == 1? main_left : tmp_left;
            }
            
            ArrayIterator getTemp(int level)
            {
                return level % 2 == 1? tmp_left : main_left;
            }
        };
        
        
        template<class ArrayIterator>
        void merge_sort_internal(MergeSortData<ArrayIterator> data, int avail_cpus, int level)
        {
            const auto size = data.size;
            assert(size > 0);

            if (size > BoseNelsonSortingNetwork<ArrayIterator>::max_items || level % 2 == 0)
            {
                const int middle = size / 2;

                const size_t size_1 = middle;
                const size_t size_2 = size - middle;

                //divide into two parts
                MergeSortData<ArrayIterator> data1, data2;
                data1.main_left = data.main_left;
                data1.tmp_left = data.tmp_left;
                data1.size = size_1;
                
                data2.main_left = data.main_left + middle;
                data2.tmp_left = data.tmp_left + middle;
                data2.size = size_2;
                
                //run
                const int cores_to_use = avail_cpus >= 2? 2: 1;   // 2 - parallel; 1 - single
                const int cores_for_sub = avail_cpus / cores_to_use;

                #pragma omp parallel sections default(shared) num_threads(cores_to_use)
                {                    
                    #pragma omp section
                        merge_sort_internal(data1, cores_for_sub, level + 1);
                        
                    #pragma omp section
                        merge_sort_internal(data2, cores_for_sub, level + 1);
                }

                //merge them
                ArrayIterator array_1 = data.getWork(level + 1);           //where level + 1 was working?
                ArrayIterator array_2 = data.getWork(level + 1) + middle;
                size_t idx_1 = 0, idx_2 = 0;
             
                ArrayIterator insert = data.getWork(level);                //where do we work?
                while (idx_1 < size_1 || idx_2 < size_2)
                {
                    const bool array2_empty = idx_2 == size_2;
                    const bool array1_not_empty = idx_1 < size_1;
                    const bool use_array1 = array2_empty || (array1_not_empty && array_1[idx_1] < array_2[idx_2]);

                    if (use_array1)
                    {
                        *insert = *(array_1 + idx_1);
                        ++idx_1;
                    }
                    else
                    {
                        *insert = *(array_2 + idx_2);
                        ++idx_2;
                    }
                    
                    ++insert;
                }
            }
            else
                fast_sort(data.getWork(level), data.getWork(level) + data.size);
        }
        

        template<class ArrayIterator>
        void merge_sort(ArrayIterator left, ArrayIterator right, int avail_cpus)
        {
            typedef typename std::remove_reference<decltype(*left)>::type data_type;
            const auto size = right - left;
            auto *tmp = new data_type[size];

            MergeSortData<ArrayIterator> data;
            data.main_left = left;
            data.tmp_left = tmp;
            data.size = size;
            
            merge_sort_internal(data, avail_cpus, 1);
            
            delete [] tmp;
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
        
    }
}

#endif
