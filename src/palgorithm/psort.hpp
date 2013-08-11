
#ifndef OPENLIBRARY_PALGORITHM_PSORT
#define OPENLIBRARY_PALGORITHM_PSORT

#include "private/psort_private.hpp"


template<class ArrayIterator>
void sort(ArrayIterator first, ArrayIterator last)
{
    OpenLibrary::Private::quick_sort(first, last, 2);
}

#endif 
