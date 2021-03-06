
/*************************************************
 *    standard types for A* routing algorithm    *
 *           Author: Michał Walenciak            *
 *           Creation date: 18.08.2012           *
 *************************************************/



#ifndef OPENLIBRARY_A_START_ROUTER_STD_TYPES_HPP
#define OPENLIBRARY_A_START_ROUTER_STD_TYPES_HPP

#include <assert.h>
#include <set>
#include <vector>
#include <iostream>


namespace ol
{
    namespace Router
    {

        template<class CoordT>
        struct Coordinates
        {
            CoordT x, y;                 //coordinates

            Coordinates(CoordT _x, CoordT _y): x(_x), y(_y) {}
            virtual ~Coordinates() {}

            //may be used by set/map:
            bool operator< (const Coordinates &other) const
            {
                bool result = false;

                //there may be some problems with floatingpoint types
                if (y < other.y)
                    result = true;
                else if (y > other.y)
                    result = false;
                else
                    result = x < other.x;

                return result;
            }

            bool operator==(const Coordinates &other) const
            {
                return x == other.x && y == other.y;
            }

            friend std::ostream& operator<<(std::ostream &stream, const Coordinates &c)
            {
               stream << c.x << ", " << c.y;

               return stream;
            }
        };

        template<class CoordT, class ScoreT>
        struct Point: Coordinates<CoordT>
        {
            ScoreT f_score, g_score;     //scores
            Point *origin;               //origin of point

            Point(const CoordT &_x, const CoordT &_y, Point *p = nullptr): Coordinates<CoordT>(_x, _y), f_score(0), g_score(0), origin(p) {}

//remove this macro when all supported compiler will support c++11
#if __cplusplus == 201103L
            Point(Point && ) = delete;
            Point(const Point &) = delete;
            virtual ~Point() {}

            Point &operator=(Point && ) = delete;
            Point &operator=(Point &) = delete;
#endif

            friend std::ostream& operator<<(std::ostream &stream, const Point &p)
            {
               stream << static_cast<const Coordinates<CoordT> &>(p);

               stream << "; q_score = " << p.g_score << ", f_score = " << p.f_score;

               return stream;
            }
        };

        //used for comparing points itself, not pointers to them
        template<class PointT>
        class CompareCoordinates
        {
            public:
                bool operator() (const PointT *const p1, const PointT *const p2) const
                {
                    return *p1 < *p2;
                }
        };


        //basic implementation of OpenSet type for A* router.
        //PointT must meet the same rules as required for 'PointT' in router.hpp
        //
        //SortedPointT is a container which implements functions:
        //* void insert(PointT *) - for adding point
        //* PointT* first() - for getting first (best) point. Function must remove point from container
        //* void clear() - for clearing container. Contained pointers cannot be destroyed
        //* size_t size() - number of elements
        template<class PointT, class SortedPointT>
        class OpenSet
        {
            public:
                OpenSet(): m_points(), m_value() {}
                virtual ~OpenSet()
                {
                    clear();
                }

                PointT *getBest()
                {
                    PointT *result = m_value.first();

                    m_points.erase(result);         //TODO: optimize it

                    assert(m_points.size() == m_value.size());

                    return result;
                }

                bool exists(const PointT *p, PointT* &ret) const
                {
                    typename std::set<PointT *>::const_iterator it = m_points.find(const_cast<PointT *>(p));
                    const bool successed = it != m_points.end();

                    if (successed)
                        ret = *it;

                    return successed;
                }

                void clear()
                {
                    //free memory
                    for(auto item: m_points)          //m_points and m_value keep the same points, delete them once
                        delete item;

                    //remove items
                    m_points.clear();
                    m_value.clear();
                }

                void insert(PointT *p)
                {
                    m_points.insert(p);
                    m_value.insert(p);

                    assert(m_points.size() == m_value.size());
                }

                bool isEmpty() const
                {
                    assert(m_points.size() == m_value.size());

                    return m_points.empty();
                }


                friend std::ostream& operator<<(std::ostream &stream, const OpenSet &openSet)
                {
                    stream << "[" << openSet.m_points.size() << "] [";

                    for (auto item: openSet.m_points)
                        stream << " (" << (*item) << ")";

                    stream << " ]";

                    return stream;
                }

            private:
                std::set<PointT *, CompareCoordinates<PointT>> m_points;     //for point fast finding
                SortedPointT m_value;            //for points sorting
        };


        //basic implementation of ClosedSet type for A* router.
        template<class PointT>
        class ClosedSet: public std::set<PointT *>
        {
            public:
                ClosedSet(): std::set<PointT *>() {}
                virtual ~ClosedSet()
                {
                    clear();
                }

                bool exists(const PointT *p) const
                {
                    typename std::set<PointT *>::const_iterator it = this->find(const_cast<PointT *>(p));
                    const bool status = it != this->end();

                    return status;
                }

                void clear()
                {
                    //free memory
                    for(auto item: *this)
                        delete item;

                    //remove items
                    std::set<PointT *>::clear();
                }

                friend std::ostream& operator<<(std::ostream &stream, const ClosedSet &closedSet)
                {
                    stream << "[" << closedSet.size() << "] [";

                    for (auto item: closedSet)
                        stream << " (" << (*item) << ")";

                    stream << " ]";

                    return stream;
                }
        };

    }
}

#endif

