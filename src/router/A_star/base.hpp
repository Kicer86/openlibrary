
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
#include <iostream>

#define DEBUG_DISABLE_OUTPUT
#include "../debug.hpp"

#include "traits.hpp"

namespace OpenLibrary
{
    namespace Router
    {

        namespace
        {
            template<class T, class U, bool>
            struct _H
            {
                static void insert(T &, const U &) {}
            };

            template<class T, class U>
            struct _H<T, U, true>
            {
                static void insert(T &container, const U &element)
                {
                    container.push_front(element);
                }
            };

            template<class T, class U>
            struct _H<T, U, false>
            {
                static void insert(T &container, const U &element)
                {
                    container.push_back(element);
                }
            };
        }

        /**
            \class AStar

            @tparam PointT:     type for points (kept in open and closed sets). Must implement:
                                * public access to variables x, and y
                                * public access to variables g_score and f_score
                                * public access to variable origin of type PointT*
                                * PointT(T x, T y, PointT *p = nullptr) - constructor with 'x' and 'y' coordinates (where T is any type you wish ;) ), and 'origin' == 'p'
                                * probably some operators to allow keeping points in sets/maps
                                * operator == which compares coordinates only
                                * friend std::ostream& operator<<(std::ostream &, const PointT &) - for debbuging reasons

            @tparam OpenSetT:   type for open points container. Must implement functions:
                                * PointT* getBest() - which returns point with lowest f_score. Function must remove point from container
                                * bool exists(const PointT *, PointT* &) - which checks if point exists in open set, and returns it if it does (second parameter)
                                * void clear() - clear container and delete point's!
                                * void insert(T *) - function for adding points. Set must take care of deletion of it's points
                                * bool isEmpty() const - function for checking if container is empty
                                * * friend std::ostream& operator<<(std::ostream &, const OpenSetT &) - for debbuging reasons

            @tparam ClosedSetT: type for closed points container. Must implement functions:
                                * void clear() - clear container and delete point's!
                                * bool exists(const Point *) - which checks if point exists in open set
                                * void insert(T *) - function for adding points. Set must take care of deletion of it's points
                                * friend std::ostream& operator<<(std::ostream &, const ClosedSetT &) - for debbuging reasons

            @tparam flags:      options
        */

        typedef unsigned int FlagsT;

        template<class PointT, class OpenSetT, class ClosedSetT, FlagsT flags = 0>
        class AStar
        {
            public:
                AStar(): m_openSet(), m_closedSet() {}
                virtual ~AStar() {}

                /**
                    \brief function for finding path
                    @tparam PathT       - container for points (coordinates) which supports push_front operator like std::list<CoordinateT>.
                                          There must exists assigment operator for CoordinateT which takes PointT.
                                          Easiest way is to inherite PointT from CoordinateT
                    @tparam CoordinateT - coordinate type (must provide two public variables: 'x' and 'y')

                    @param startPoint   - first point
                    @param endPoint     - last point
                  */

                template<class PathT, class CoordinateT>
                PathT route(CoordinateT startPoint, CoordinateT endPoint)
                {
                    if (Container<PathT>::prependable == false)    //if containter is not prependable - switch start and end points
                        std::swap(startPoint, endPoint);

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

                //main loop of algorithm
                virtual bool findPath(PointT *endPoint)
                {
                    bool status = false;

                    while (m_openSet.isEmpty() == false)
                    {
                        debug(DebugLevel::Debug) << "\nopen set: " << m_openSet;
                        debug(DebugLevel::Debug) << "closed set: " << m_closedSet;
                        PointT *currentPoint = m_openSet.getBest();

                        debug(DebugLevel::Debug) << "current point: " << *currentPoint;

                        if ( *currentPoint == *endPoint )
                        {
                            debug(DebugLevel::Debug) << "\t== end point";
                            endPoint->origin = currentPoint->origin;
                            endPoint->f_score = currentPoint->f_score;
                            endPoint->g_score = currentPoint->g_score;
                            status = true;
                            break;
                        }

                        m_closedSet.insert(currentPoint);

                        const std::vector<PointT *> neighbours = get_neighbours(currentPoint);
                        debug(DebugLevel::Debug) << "\tneighbours: " << neighbours;

                        for (PointT *neighbour: neighbours)
                        {
                            bool saved = false;
                            debug(DebugLevel::Debug) << "\tprocessing neigbhbour " << *neighbour;

                            //check if neigbhbour is already processed
                            if (m_closedSet.exists(neighbour))
                                debug(DebugLevel::Debug) << "\t\tin closed set";
                            else
                            {
                                const GScoreT neighbour_g_score = currentPoint->g_score + distance(currentPoint, neighbour);

                                static PointT dummy(0 ,0);
                                PointT *existing = &dummy;

                                //check if in open set
                                if (m_openSet.exists(neighbour, existing) == false || neighbour_g_score < existing->g_score)
                                {
                                    if (existing != &dummy)  //just update existing point
                                    {
                                        debug(DebugLevel::Debug) << "\t\tbetter than point already existing in open set. Updating";
                                        existing->origin = neighbour->origin;
                                        existing->g_score = neighbour_g_score;

                                        //due to potencial complexity of heuristic_cost_estimate function, recalculate f_score
                                        existing->f_score = heuristic_cost_estimate(existing, endPoint);
                                    }
                                    else
                                    {
                                        debug(DebugLevel::Debug) << "\t\tnot in open set. Adding";
                                        neighbour->g_score = neighbour_g_score;
                                        neighbour->f_score = heuristic_cost_estimate(neighbour, endPoint);
                                        //neighbour->origin already set

                                        //add new point or update existing one
                                        m_openSet.insert(neighbour);
                                        saved = true;
                                    }
                                }
                                else
                                    debug(DebugLevel::Debug) << "\t\talready in open set";
                            }

                            if (saved == false)
                                delete neighbour;
                        }
                    }

                    return status;
                }

                virtual FScoreT heuristic_cost_estimate(const PointT *p1, const PointT *p2) const = 0;
                virtual std::vector<PointT *> get_neighbours(PointT *p) = 0;

                template<class PathT>
                PathT reconstruct_path(PointT *last)
                {
                    assert(last->origin != nullptr);

                    PathT result;
                    PointT *p = last;

                    while (p != nullptr)
                        _H<PathT, PointT, Container<PathT>::prependable>::insert(result, *p), p = p->origin;

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
