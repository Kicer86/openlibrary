
#ifndef SORTING_NETWORKS_HPP
#define SORTING_NETWORKS_HPP


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

//bose-nelson algorithm based on:
// http://www.drdobbs.com/sorting-networks/184402663?pgno=2

        template<class ArrayIterator>
        class BoseNelsonSortingNetwork
        {
            public:
                static constexpr int max_items = 32;

                BoseNelsonSortingNetwork(ArrayIterator arrayBegin): m_array(arrayBegin) {}

                //sort array provided in constructor which is of size 'size'
                template<size_t size>
                void sort() const
                {
                    bose<size>();
                }

            private:
                ArrayIterator m_array;

                template<size_t i, size_t j>
                void P() const
                {
                    //printf("swap(%lu, %lu);\n", i, j);
                    sort_swap(m_array + i, m_array + j);
                }

                template<size_t i, size_t x, size_t j, size_t y>
                void Pbracket() const
//int i;  /* value of first element in sequence 1 */
//int x;  /* length of sequence 1 */
//int j;  /* value of first element in sequence 2 */
//int y;  /* length of sequence 2 */
                {
                    if(x == 1 && y == 1)
                        P<i, j>(); /* 1 comparison sorts 2 items */
                    else if(x == 1 && y == 2)
                    {
                        /* 2 comparisons inserts an item into an
                         * already sorted sequence of length 2. */
                        P < i, (j + 1) > ();
                        P<i, j>();
                    }
                    else if(x == 2 && y == 1)
                    {
                        /* As above, but inserting j */
                        P<i, j>();
                        P < (i + 1), j > ();
                    }
                    else
                    {
                        /* Recurse on shorter sequences, attempting
                         * to make the length of one subsequence odd
                         * and the length of the other even. If we
                         * can do this, we eventually merge the two. */
                        constexpr size_t a = x / 2;
                        constexpr size_t b = (x & 1) ? (y / 2) : ((y + 1) / 2);
                        Pbracket<i, a, j, b>();
                        Pbracket < (i + a), (x - a), (j + b), (y - b) > ();
                        Pbracket < (i + a), (x - a), j, b > ();
                    }
                }

                template<size_t i, size_t m>
                inline void Pstar() const
//int i;  /* value of first element in sequence */
//int m;  /* length of sequence */
                {
                    if(m > 1)
                    {
                        /* Partition into 2 shorter sequences,
                         * generate a sorting method for each,
                         * and merge the two sub-networks. */
                        constexpr size_t a = m / 2;
                        Pstar<i, a>();
                        Pstar < (i + a), (m - a) > ();
                        Pbracket < i, a, (i + a), (m - a) > ();
                    }
                }

                /* Calling bose(n) generates a network
                 * to sort n items. See R. C. Bose & R. J. Nelson,
                 * "A Sorting Problem", JACM Vol. 9, Pp. 282-296. */
                template<size_t n>
                inline void bose() const
                {
                    //Pstar(1, n); /* sort the sequence {X1,...,Xn} */
                    Pstar<0, n>(); //like in original but start from 0
                }
        };


// sorting network:
// http://pages.ripco.net/~jgamble/nw.html
// http://en.wikipedia.org/wiki/Sorting_network
// parser: sed -e "s@\[\[@\[@g" -e "s@\]\]@\]@g" -e "s@\],\[@\]\[@g" -e "s@\[\([0-9]\+\),\([0-9]\+\)\]@sort_swap<T>(array, \1, \2);\n@g" input

        template<class T>
        class JumpTable
        {
                typedef void (BoseNelsonSortingNetwork<T>::*SortingFunction)() const;

            public:
                JumpTable(): m_jumpTable(new SortingFunction[BoseNelsonSortingNetwork<T>::max_items + 1])
                {
                    Generator<T, BoseNelsonSortingNetwork<T>::max_items>().generate(m_jumpTable);
                }

                void call(BoseNelsonSortingNetwork<T> &boseNelson, int idx) const
                {
                    assert(idx <= BoseNelsonSortingNetwork<T>::max_items);

                    SortingFunction sort = m_jumpTable[idx];
                    (boseNelson.*sort)();
                }

            private:
                SortingFunction* m_jumpTable;

                template<class P, int iteration>
                struct Generator
                {
                    inline void generate(SortingFunction* table) const
                    {
                        Generator < P, iteration - 1 > ().generate(table);
                        table[iteration] = &BoseNelsonSortingNetwork<T>::template sort<iteration>;
                    }
                };

                template<class P>
                struct Generator < P, -1 >
                {
                    inline void generate(SortingFunction*) const
                    {
                    }
                };

        };

    }
}

#endif
