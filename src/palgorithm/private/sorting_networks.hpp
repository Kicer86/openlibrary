
#ifndef SORTING_NETWORKS_HPP
#define SORTING_NETWORKS_HPP

#include <assert.h>

#include <algorithm>
#include <vector>
#include <array>

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
        class SortingNetworksSort
        {

            public:
                typedef std::array<typename BoseNelsonSortingNetworkGenerator<ArrayIterator>::SwapInstructions, 
                                   BoseNelsonSortingNetworkGenerator<ArrayIterator>::max_items + 1> InstructionsSet;
                                   
                SortingNetworksSort(): m_swapInstructions()
                {
                    Generator<ArrayIterator, BoseNelsonSortingNetworkGenerator<ArrayIterator>::max_items>().generate(m_swapInstructions);
                }

                void sort(ArrayIterator left, ArrayIterator right) const
                {
                    const auto size = right - left;

                    assert(size <= BoseNelsonSortingNetworkGenerator<ArrayIterator>::max_items);

                    const typename BoseNelsonSortingNetworkGenerator<ArrayIterator>::SwapInstructions& instructions = m_swapInstructions[size];
                    
                    for(const auto &instruction: instructions)
                        sort_swap(left + instruction.first, left + instruction.second);
                }

            private:
                InstructionsSet m_swapInstructions;

                template<class P, int iteration>
                struct Generator
                {
                    inline void generate(InstructionsSet &inst_table) const
                    {
                        Generator < P, iteration - 1 > ().generate(inst_table);
			
                        BoseNelsonSortingNetworkGenerator<ArrayIterator> boseNelson(inst_table[iteration]);
                        boseNelson.template generate<iteration>();
                    }
                };

                template<class P>
                struct Generator < P, -1 >
                {
                    inline void generate(InstructionsSet &) const
                    {
                    }
                };

        };

    }
}

#endif
