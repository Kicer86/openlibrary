/*
    Router visualisation
    Copyright (C) 2011  Michał Walenciak <Kicer86@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <QPaintEvent>
#include <QPainter>

#include "routerdemo.hpp"


#include "A_star/std_alg.hpp"

#include <set>
#include <forward_list>
namespace
{
    using namespace OpenLibrary::Router;
    typedef Point<int, double> PointT;
    typedef Coordinates<int> CoordinatesT;
    //typedef OpenSet<PointT> OpenSetT;
    typedef ClosedSet<PointT> ClosedSetT;
    StdAStar<PointT, ClosedSetT> router;

    static class r
    {
        public:
            r()
            {
                typedef std::forward_list<CoordinatesT> PathT;
                Coordinates<int> s(0, 0), e(9, 9);
                PathT path = router.route<PathT>(s, e);

                (void) path;

                exit(0);
            }
    } _run;
}

