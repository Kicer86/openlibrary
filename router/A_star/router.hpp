
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
                             *

            \arg OpenSetT:   type for open points container. Must implement functions:
                             * PointT getBest() - which returns point with lowest f_score
                             * bool exists(PointT) - which checks if point exists in open set
                             *

            \arg ClosedSetT: type for closed points container. Must implement functions:
                             *

            \arg
        */

        typedef unsigned int FlagsT;

        template<class PointT, class OpenSetT, class ClosedSetT, FlagsT flags = 0>
        class AStar
        {
            public:
                AStar() {}
                virtual ~AStar() {}

                template<class T>
                bool route(const T &startPoint, const T &endPoint)
                {
                    const bool status = route(startPoint, endPoint);
                    return status;
                }

            private:
                OpenSetT  m_openSet;
                ClosedSetT m_closedSet;

                bool route(const PointT &startPoint, const PointT &endPoint)
                {

                }
        };
    }
}

#endif
