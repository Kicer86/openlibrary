
#include "A_star/std_alg.hpp"

#include <set>
#include <vector>


int main(int /*argc*/, char **/*argv*/)
{
    using namespace OpenLibrary::Router;
    typedef Point<int, double> PointT;
    typedef Coordinates<int> CoordinatesT;
    //typedef OpenSet<PointT> OpenSetT;
    typedef ClosedSet<PointT> ClosedSetT;
    StdAStar<PointT, ClosedSetT> router;

    typedef std::vector<CoordinatesT> PathT;
    Coordinates<int> s(0, 0), e(9, 9);
    PathT path = router.route<PathT>(s, e);

    (void) path;

    return 0;
}
