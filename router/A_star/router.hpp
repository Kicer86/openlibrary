
/***************************************
 *    Base for A* routing algorithm    *
 *      Author: Micha³ Walenciak       *
 *      Creation date: 14.08.2012      *
 ***************************************/

#ifndef OPENLIBRARY_A_START_ROUTER_HPP
#define OPENLIBRARY_A_START_ROUTER_HPP

namespace OpenLibrary
{
    namespace Router
    {
        /**
            \class AStar

            \arg PointT:     type for points (kept in open and closed sets). Must implement:
                             * public access to variables x, and y
                             * public access to variables g_score and f_score
                             * PointT(T x, T y) - constructor with 'x' and 'y' coordinates (where T is any type you wish ;) )

            \arg OpenSetT:   type for open points container. Must implement functions:
                             * PointT getBest() - which returns point with lowest f_score
                             * bool exists(PointT) - which checks if point exists in open set
                             * void clear() - clear container
                             * voin insert(const T &) - function for adding points

            \arg ClosedSetT: type for closed points container. Must implement functions:
                             * void clear() - clear container
                             * voin insert(const T &) - function for adding points

            \arg flags:      options
        */

        typedef unsigned int FlagsT;

        template<class PointT, class OpenSetT, class ClosedSetT, FlagsT flags = 0>
        class AStar
        {
            public:
                AStar() {}
                virtual ~AStar() {}

                template<class CoordinateT>
                bool route(const CoordinateT &startPoint, const CoordinateT &endPoint)
                {
                    //prepare sets
                    m_openSet.clear();
                    m_closedSet.clear();

                    PointT firstPoint(startPoint.x, startPoint.y);
                    PointT lastPoint(endPoint.x, endPoint.y);
                    firstPoint.g_score = 0;
                    m_openSet.insert(firstPoint);

                    //find path
                    const bool status = findPath(lastPoint);

                    return status;
                }

            private:
                OpenSetT  m_openSet;
                ClosedSetT m_closedSet;

                bool findPath(const PointT &&endPoint)
                {

                }
        };
    }
}

#endif
