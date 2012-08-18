
/*************************************************
 *    standard types for A* routing algorithm    *
 *           Author: Micha³ Walenciak            *
 *           Creation date: 18.08.2012           *
 *************************************************/



#ifndef OPENLIBRARY_A_START_ROUTER_STD_TYPES_HPP
#define OPENLIBRARY_A_START_ROUTER_STD_TYPES_HPP

namespace OpenLibrary
{
    namespace Router
    {

        template<class CoordT>
        struct Coordinates
        {
            CoordT x, y;                 //coordinates

            Coordinates(CoordT _x, CoordT _y): x(_x), y(_y) {}
            virtual ~Coordinates() {}
        };

        template<class CoordT, class ScoreT>
        struct Point: Coordinates<CoordT>
        {
            ScoreT f_score, g_score;     //scores
            Point *origin;               //origin of point

            Point(const CoordT &_x, const CoordT &_y): Coordinates<CoordT>(_x, _y), f_score(0), g_score(0), origin(nullptr) {}
            Point(Point && ) = delete;
            Point(const Point &) = delete;
            Point &operator=(Point && ) = delete;
            Point &operator=(Point &) = delete;
            virtual ~Point() {}
        };
    }
}

#endif
