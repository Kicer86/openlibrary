
#ifndef BOSE_NELSON_HPP
#define BOSE_NELSON_HPP

#include <algorithm>
#include <vector>


//bose-nelson algorithm based on:
// http://www.drdobbs.com/sorting-networks/184402663?pgno=2

// sorting network:
// http://pages.ripco.net/~jgamble/nw.html
// http://en.wikipedia.org/wiki/Sorting_network
// parser: sed -e "s@\[\[@\[@g" -e "s@\]\]@\]@g" -e "s@\],\[@\]\[@g" -e "s@\[\([0-9]\+\),\([0-9]\+\)\]@sort_swap<T>(array, \1, \2);\n@g" input


class BoseNelsonSortingNetworkGenerator
{
    public:
        static constexpr int max_items = 32;
        typedef std::pair<int, int> SwapInfo;
        typedef std::vector<SwapInfo> SwapInstructions;

        constexpr BoseNelsonSortingNetworkGenerator(SwapInstructions &v): m_swapInstr(v) {}

        //generate sort instructions
        void generate(size_t size) const
        {
            bose(size);
        }

    private:
        SwapInstructions &m_swapInstr;

        void P(size_t i, size_t j) const
        {
            //printf("swap(%lu, %lu);\n", i, j);
            SwapInfo info(i, j);
            m_swapInstr.push_back(info);
        }

        void Pbracket(size_t i, size_t x, size_t j, size_t y) const
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

        void Pstar(size_t i, size_t m) const
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

        /* Calling bose(n) generates a network
            * to sort n items. See R. C. Bose & R. J. Nelson,
            * "A Sorting Problem", JACM Vol. 9, Pp. 282-296. */

        void bose(size_t n) const
        {
            //Pstar(1, n); /* sort the sequence {X1,...,Xn} */
            Pstar(0, n); //like in original but start from 0
        }
};
        
#endif
