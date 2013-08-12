
#ifndef PSORT_PRIVATE
#define PSORT_PRIVATE

namespace OpenLibrary
{
    namespace Private
    {

        template<class ArrayIterator>
        inline void sort_swap(ArrayIterator i1, ArrayIterator i2)
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
                static constexpr size_t max_items = 32;

                BoseNelsonSortingNetwork(ArrayIterator arrayBegin): m_array(arrayBegin) {}

                //sort array provided in constructor which is of size 'size'
                template<size_t size>
                inline void sort() const
                {
                    bose<size>();
                }

            private:
                ArrayIterator m_array;

                template<size_t i, size_t j>
                inline void P() const
                {
                    //printf("swap(%lu, %lu);\n", i, j);
                    sort_swap(m_array + i, m_array + j);
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

                inline void call(BoseNelsonSortingNetwork<T> &boseNelson, size_t idx) const
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
        ArrayIterator partition(ArrayIterator left, ArrayIterator right, ArrayIterator pivotIndex) __attribute__((noinline));
        
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
        ArrayIterator pivotIdx(ArrayIterator left, ArrayIterator right) __attribute__((noinline));
        
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

        /*
        void quick_sort1(int* array, size_t size) __attribute__((noinline));
        void quick_sort1(int* array, size_t size)
        {
            if (size > BoseNelsonSortingNetwork<int>::max_items)
            {
                //std::cout << "partitioning array of size " << size << std::endl;
                int *pivot_tmp = pivotIdx(&array[0], &array[size - 1]);
                size_t pivot = pivot_tmp - &array[0];
                
                int *div_tmp = partition(&array[0], &array[size - 1], &array[pivot]);
                size_t div = div_tmp - &array[0];

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
        */

        template<class ArrayIterator>
        void quick_sort(ArrayIterator left, ArrayIterator right, int avail_cpus) __attribute__((noinline));
        
        template<class ArrayIterator>
        void quick_sort(ArrayIterator left, ArrayIterator right, int avail_cpus)
        {
            assert(avail_cpus > 0);
            const auto size = right - left;
            
            if (size > BoseNelsonSortingNetwork<int>::max_items)
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
        

        void merge_sort(int* array, size_t size) __attribute__((noinline));
        void merge_sort(int* array, size_t size)
        {
            assert(size > 0);

            if (size == 2)
            {
                if (array[0] > array[1])
                    std::swap(array[0], array[1]);
            }
            else if (size > 2)
            {
                const int middle = size / 2;

                const size_t size_1 = middle;
                const size_t size_2 = size - middle;
                int* array_1 = array;
                int* array_2 = array + middle;

                //divide into two parts

                merge_sort(array, middle);
                merge_sort(array + middle, size - middle);

                //merge them
                size_t idx_1 = 0, idx_2 = 0;
                size_t inserter = 0;

                int* tmp = new int[size];

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
        
    }
}

#endif
