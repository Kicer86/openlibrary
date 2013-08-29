
#ifndef PSORT_PRIVATE
#define PSORT_PRIVATE

#include <assert.h>
#include <omp.h>
#include <algorithm>
#include <type_traits>

#include "swap_tables.hpp"

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


        template<class ArrayIterator>
        void fast_sort(ArrayIterator left, ArrayIterator right)
        {
            const auto items = right - left;
            
            assert(items <= BoseNelson::max_items);
            
            if (items <= BoseNelson::max_items)
            {
                const int swapInstructionsSteps = BoseNelson::instructionsTableSize[items];
                const int* swapsList = BoseNelson::instructionsTable[items];
            
                for(int i = 0; i < swapInstructionsSteps; i++)
                {
                    const size_t item_left = swapsList[i * 2];
                    const size_t item_right = swapsList[i * 2 + 1];
                    
                    sort_swap(left + item_left, left + item_right);
                }
            }


            assert(items == 0 || std::is_sorted(left, right));
        }


        /*****************************************************************************/
        /*                                  PQSORT                                   */
        /*****************************************************************************/

// http://pl.wikipedia.org/wiki/Sortowanie_introspektywne

// left is the index of the leftmost element of the array
// right is the index of the rightmost element of the array (inclusive)
// number of elements in subarray = right-left

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
            
            if (size > BoseNelson::max_items)
            {
                //std::cout << "partitioning array of size " << size << std::endl;
                ArrayIterator pivot = pivotIdx(left, right);                                
                ArrayIterator div = partition(left, right, pivot);
                
                const auto div_pos = div - left;
                
                int cores_to_use = avail_cpus >= 2? 2: 1;   // 2 - parallel; 1 - single
                int cores_for_sub = avail_cpus / cores_to_use;
		
		const int size1 = div_pos;
		const int size2 = size - div_pos;
		
		//when one of the parts is much more bigger than another, do not use parallelism here but propagate it down
		if (size1 / 2 >= size2 || size2 / 2 >= size1)
		{
		    cores_to_use = 1;
		    cores_for_sub = avail_cpus;
		}

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

            if (size > BoseNelson::max_items || level % 2 == 0)
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
                    const bool use_array1 = array2_empty || (array1_not_empty && array_1[idx_1] <= array_2[idx_2]);   //<= guaranties that merge sort will be stable

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
