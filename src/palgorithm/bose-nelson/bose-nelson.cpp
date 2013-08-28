
#include "bose-nelson.hpp"

BoseNelsonSortingNetworkGenerator::BoseNelsonSortingNetworkGenerator(SwapInstructions& v): m_swapInstr(v) 
{
    
}

void BoseNelsonSortingNetworkGenerator::generate(size_t size) const
{
    bose(size);
}


void BoseNelsonSortingNetworkGenerator::P(size_t i, size_t j) const
{
    //printf("swap(%lu, %lu);\n", i, j);
    m_swapInstr.push_back(i);
    m_swapInstr.push_back(j);
}


void BoseNelsonSortingNetworkGenerator::Pbracket(size_t i, size_t x, size_t j, size_t y) const

//int i;  /* value of first element in sequence 1 */
//int x;  /* length of sequence 1 */
//int j;  /* value of first element in sequence 2 */
//int y;  /* length of sequence 2 */
        
{
    if(x == 1 && y == 1)
        P(i, j); /* 1 comparison sorts 2 items */
    else if(x == 1 && y == 2)
    {
        /* 2 comparisons inserts an item into an
            * already sorted sequence of length 2. */
        P(i, (j + 1));
        P(i, j);
    }
    else if(x == 2 && y == 1)
    {
        /* As above, but inserting j */
        P(i, j);
        P((i + 1), j);
    }
    else
    {
        /* Recurse on shorter sequences, attempting
            * to make the length of one subsequence odd
            * and the length of the other even. If we
            * can do this, we eventually merge the two. */
        const size_t a = x / 2;
        const size_t b = (x & 1) ? (y / 2) : ((y + 1) / 2);
        Pbracket(i, a, j, b);
        Pbracket((i + a), (x - a), (j + b), (y - b));
        Pbracket((i + a), (x - a), j, b);
    }
}


void BoseNelsonSortingNetworkGenerator::Pstar(size_t i, size_t m) const
//int i;  /* value of first element in sequence */
//int m;  /* length of sequence */        
{
    if(m > 1)
    {
        /* Partition into 2 shorter sequences,
            * generate a sorting method for each,
            * and merge the two sub-networks. */
        const size_t a = m / 2;
        Pstar(i, a);
        Pstar((i + a), (m - a));
        Pbracket(i, a, (i + a), (m - a));
    }
}
void BoseNelsonSortingNetworkGenerator::bose(size_t n) const
{
    //Pstar(1, n); /* sort the sequence {X1,...,Xn} */
    Pstar(0, n); //like in original but start from 0
}
