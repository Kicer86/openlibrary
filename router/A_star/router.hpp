
/***************************************
 *    Base for A* routing algorithm    *
 *      Author: Michał Walenciak       *
 *      Creation date: 14.08.2012      *
 ***************************************/

#ifndef OPENLIBRARY_A_START_ROUTER_HPP
#define OPENLIBRARY_A_START_ROUTER_HPP

#include <cmath>

namespace OpenLibrary
{
    namespace Router
    {
        /**
            \class AStar

            \arg PointT:       type for points (kept in open and closed sets). Must implement:
                               * public access to variables x, and y
                               * public access to variables g_score and f_score
                               * public access to variable origin of type PointT*
                               * PointT(T x, T y) - constructor with 'x' and 'y' coordinates (where T is any type you wish ;) )
                               * probably some operators to allow keeping points in sets/maps

            \arg OpenSetT:     type for open points container. Must implement functions:
                               * PointT getBest() - which returns point with lowest f_score
                               * bool exists(PointT) - which checks if point exists in open set
                               * void clear() - clear container and delete point's!
                               * void insert(T *) - function for adding points. Set must take care of deletion of it's points

            \arg ClosedSetT:   type for closed points container. Must implement functions:
                               * void clear() - clear container and delete point's!
                               * voin insert(T *) - function for adding points. Set must take care of deletion of it's points

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
                    PointT firstPoint(startPoint.x, startPoint.y);
                    PointT lastPoint(endPoint.x, endPoint.y);

                    //prepare sets
                    init(&firstPoint);

                    //find path
                    const bool status = findPath(&lastPoint);

                    return status;
                }

            protected:
                typedef decltype(PointT::f_score) FScoreT;

                OpenSetT  m_openSet;
                ClosedSetT m_closedSet;

                virtual void init(PointT *startPoint)
                {
                    m_openSet.clear();
                    m_closedSet.clear();
                    m_openSet.insert(startPoint);
                }

                virtual bool findPath(PointT *endPoint)
                {

                }

                virtual FScoreT heuristic_cost_estimate(const PointT *p1, const PointT *p2)
                {
                    const auto pow2 = [](long double x) -> long double { return x * x; };

                    FScoreT dist = sqrt( pow2(p1->x - p2->x) + pow2(p1->y - p2->y) );

                    return dist;
                }
        };
    }
}

#endif
