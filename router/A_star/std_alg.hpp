
/**************************************************
 *    standard version of A* routing algorithm    *
 *           Author: Michał Walenciak             *
 *           Creation date: 23.08.2012            *
 **************************************************/

#include "base.hpp"
#include "std_types.hpp"

namespace OpenLibrary
{
    namespace Router
    {
        template<class PointT, class OpenSetT, class ClosedSetT, FlagsT flags = 0>
        class StdAStar: public AStar<PointT, OpenSetT, ClosedSetT, flags>
        {
            public:
                StdAStar() {}
                virtual ~StdAStar() {}

            protected:
                typedef AStar<PointT, OpenSetT, ClosedSetT, flags> AStarBase;

                virtual typename AStarBase::FScoreT heuristic_cost_estimate(const PointT *p1, const PointT *p2) const
                {
                    const typename AStarBase::FScoreT dist = AStarBase::distance(p1, p2);

                    return dist;
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
        };
    }
}
