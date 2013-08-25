
#include "psort.hpp"

int main()
{
    int tab[2] = {5, 2};
    
    fast_sort(&tab[0], &tab[2]);
    
    return tab[0];
}
