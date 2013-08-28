
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
        typedef std::vector<int> SwapInstructions;

        BoseNelsonSortingNetworkGenerator(SwapInstructions &v);

        //generate sort instructions
        void generate(size_t size) const;

    private:
        SwapInstructions &m_swapInstr;

        void P(size_t i, size_t j) const;
        void Pbracket(size_t i, size_t x, size_t j, size_t y) const;
        void Pstar(size_t i, size_t m) const;

        /* Calling bose(n) generates a network
            * to sort n items. See R. C. Bose & R. J. Nelson,
            * "A Sorting Problem", JACM Vol. 9, Pp. 282-296. */

        void bose(size_t n) const;
};
        
#endif
