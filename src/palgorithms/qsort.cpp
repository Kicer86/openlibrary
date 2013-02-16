
#include <algorithm>
#include <iostream>
#include <omp.h>

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


void quick_sort1(int *array, size_t size)
{
    if (size > 1)
    {
//         std::cout << "partitioning array of size " << size << std::endl;
        const size_t pivot = size / 2;
        size_t div = partition(array, 0, size - 1, pivot);


            {
                if (div > 1)
                {
//                     std::cout << "analyzing sub array of size " << div << " by thread #" << omp_get_thread_num() << std::endl;
                    quick_sort1(array, div);
                }
            }


            {
                if (div < (size - 2) )
                {
//                     std::cout << "analyzing sub array of size " << size - div << " by thread #" << omp_get_thread_num() << std::endl;
                    quick_sort1(&array[div + 1], size - div - 1);
                }
            }

    }
}


void quick_sort(int *array, size_t size)
{
    if (size > 1)
    {
//         std::cout << "partitioning array of size " << size << std::endl;
        const size_t pivot = size / 2;
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


int main()
{
    const int n = 200000000;
    int *array = new int[n];

    for (int i = 0; i < n; i++)
        array[i] = n - i;

    quick_sort(array, n);

    bool ok = true;
    for (int i = 0; i < n - 1; i++)
    {
        if (array[i] > array[i + 1])
            ok = false;
    }

//     for (int i = 0; i < n; i++)
//         std::cout << array[i] << std::endl;
//
    if (ok == false)
        std::cerr << "blad !!!" << std::endl;

    return 0;
}
