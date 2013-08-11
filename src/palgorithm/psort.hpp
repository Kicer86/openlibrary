
#ifndef OPENLIBRARY_PALGORITHM_PSORT
#define OPENLIBRARY_PALGORITHM_PSORT

#include <omp.h>

#include "private/psort_private.hpp"


template<class ArrayIterator>
void sort(ArrayIterator first, ArrayIterator last)
{
    const int cores = omp_get_num_threads();
    OpenLibrary::Private::quick_sort(first, last, cores);
}

#endif 
