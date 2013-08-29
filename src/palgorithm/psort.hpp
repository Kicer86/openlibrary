
#ifndef OPENLIBRARY_PALGORITHM_PSORT
#define OPENLIBRARY_PALGORITHM_PSORT

#include <omp.h>

#include "private/psort_private.hpp"


template<class ArrayIterator>
void sort(ArrayIterator first, ArrayIterator last)
{
    const int cores = omp_get_num_procs();
    OpenLibrary::Private::quick_sort(first, last, cores);
}

template<class ArrayIterator>
void merge_sort(ArrayIterator first, ArrayIterator last)
{
    const int cores = omp_get_num_procs();
    OpenLibrary::Private::merge_sort(first, last, cores);
}

template<class ArrayIterator>
void fast_sort(ArrayIterator first, ArrayIterator last)
{
    OpenLibrary::Private::fast_sort(first, last);
}

#endif 