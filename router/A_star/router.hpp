
/***************************************
 *    Base for A* routing algorithm    *
 *      Author: Michał Walenciak       *
 *      Creation date: 14.08.2012      *
 ***************************************/

#ifndef OPENLIBRARY_A_START_ROUTER_HPP
#define OPENLIBRARY_A_START_ROUTER_HPP

#include <assert.h>
#include <cmath>
#include <vector>

namespace OpenLibrary
{
    namespace Router
    {
        /**
            \class AStar

            @tparam PointT:     type for points (kept in open and closed sets). Must implement:
                                * public access to variables x, and y
                                * public access to variables g_score and f_score
                                * public access to variable origin of type PointT*
                                * PointT(T x, T y, PointT *p = nullptr) - constructor with 'x' and 'y' coordinates (where T is any type you wish ;) ), and 'origin' == 'p'
                                * probably some operators to allow keeping points in sets/maps
                                * operator == which compares coordinates only

            @tparam OpenSetT:   type for open points container. Must implement functions:
                                * PointT getBest() - which returns point with lowest f_score. Function must remove point from container
                                * bool exists(const PointT *, PointT* &) - which checks if point exists in open set, and returns it if it does (second parameter)
                                * void clear() - clear container and delete point's!
                                * void insert(T *) - function for adding points. Set must take care of deletion of it's points
                                * bool isEmpty() const - function for checking if container is empty

            @tparam ClosedSetT: type for closed points container. Must implement functions:
                                * void clear() - clear container and delete point's!
                                * bool exists(const Point *) - which checks if point exists in open set
                                * void insert(T *) - function for adding points. Set must take care of deletion of it's points

            @tparam flags:      options
        */

        typedef unsigned int FlagsT;

        template<class PointT, class OpenSetT, class ClosedSetT, FlagsT flags = 0>
        class AStar
        {
            public:
                AStar() {}
                virtual ~AStar() {}

                /**
                    \brief function for finding path
                    @tparam PathT       - container for points (coordinates) which supports push_front operator
                    @tparam CoordinateT - coordinate type (must provide two public variables: 'x' and 'y')

                    @param startPoint   - first point
                    @param endPoint     - last point
                  */

                template<class PathT, class CoordinateT>
                PathT route(const CoordinateT &startPoint, const CoordinateT &endPoint)
                {
                    PointT firstPoint(startPoint.x, startPoint.y);
                    PointT lastPoint(endPoint.x, endPoint.y);

                    //prepare sets
                    init(&firstPoint, &lastPoint);

                    //find path
                    const bool status = findPath(&lastPoint);

                    PathT path;
                    if (status)
                        path = reconstruct_path<PathT>(&lastPoint);

                    return std::move(path);
                }

            protected:
                typedef decltype(PointT::f_score) FScoreT;
                typedef decltype(PointT::g_score) GScoreT;

                OpenSetT  m_openSet;
                ClosedSetT m_closedSet;

                virtual void init(PointT *startPoint, PointT *endPoint)
                {
                    startPoint->f_score = heuristic_cost_estimate(startPoint, endPoint);

                    m_openSet.clear();
                    m_closedSet.clear();
                    m_openSet.insert(startPoint);
                }

                virtual bool findPath(PointT *endPoint)
                {
                    bool status = false;

                    while (m_openSet.isEmpty() == false)
                    {
                        PointT *currentPoint = m_openSet.getBest();

                        if ( *currentPoint == *endPoint )
                        {
                            endPoint->origin = currentPoint->origin;
                            endPoint->f_score = currentPoint->f_score;
                            endPoint->g_score = currentPoint->g_score;
                            status = true;
                            break;
                        }

                        m_closedSet.insert(currentPoint);

                        const std::vector<PointT *> neighbours = get_neighbours(currentPoint);

                        for (PointT *neighbour: neighbours)
                        {
                            //check if any of tenatives is already processed
                            if (m_closedSet.exists(neighbour))
                                continue;

                            const GScoreT neighbour_g_score = currentPoint->g_score + distance(currentPoint, neighbour);

                            static PointT dummy(0 ,0);
                            PointT *existing = &dummy;

                            if (m_openSet.exists(neighbour, existing) == false ||
                                neighbour_g_score < existing->g_score)
                            {
                                neighbour->g_score = neighbour_g_score;
                                neighbour->f_score = heuristic_cost_estimate(neighbour, endPoint);
                                //neighbour->origin already set

                                //add new point or update existing one
                                m_openSet.insert(neighbour);
                            }

                        }
                    }

                    return status;
                }

                virtual FScoreT heuristic_cost_estimate(const PointT *p1, const PointT *p2) const
                {
                    const FScoreT dist = distance(p1, p2);

                    return dist;
                }

                template<class PathT>
                PathT reconstruct_path(PointT *last)
                {
                    assert(last->origin != nullptr);

                    PathT result;
                    PointT *p = last;

                    while (p != nullptr)
                        result.push_front(*p), p = p->origin;

                    return std::move(result);
                }

                virtual std::vector<PointT *> get_neighbours(PointT *p)
                {
                    std::vector<PointT *> result;
                    result.reserve(8);

                    result.push_back( new PointT(p->x + 0, p->y - 1, p) );
                    result.push_back( new PointT(p->x + 1, p->y - 1, p) );
                    result.push_back( new PointT(p->x + 1, p->y + 0, p) );
                    result.push_back( new PointT(p->x + 1, p->y + 1, p) );
                    result.push_back( new PointT(p->x + 0, p->y + 1, p) );
                    result.push_back( new PointT(p->x - 1, p->y + 1, p) );
                    result.push_back( new PointT(p->x - 1, p->y + 0, p) );
                    result.push_back( new PointT(p->x - 1, p->y - 1, p) );

                    return std::move(result);
                }

                long double distance(const PointT *p1, const PointT *p2) const
                {
                    const auto pow2 = [](long double x) -> long double { return x * x; };
                    const long double dist = sqrt( pow2(p1->x - p2->x) + pow2(p1->y - p2->y) );
                    return dist;
                }
        };
    }
}

#endif
