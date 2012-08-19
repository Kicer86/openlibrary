
/*************************************************
 *    standard types for A* routing algorithm    *
 *           Author: Michał Walenciak            *
 *           Creation date: 18.08.2012           *
 *************************************************/



#ifndef OPENLIBRARY_A_START_ROUTER_STD_TYPES_HPP
#define OPENLIBRARY_A_START_ROUTER_STD_TYPES_HPP

#include <set>

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


	//basic implementation of OpenSet type for A* router.
	template<class PointT>
	class OpenSet
	{
	    public:
		OpenSet() {}
		virtual ~OpenSet() {}
	};


        //basic implementation of ClosedSet type for A* router.
        template<class PointT>
        class ClosedSet: public std::set<PointT *>
        {
            public:
                ClosedSet(): std::set<PointT *>() {}
                virtual ~ClosedSet()
                {
		    for(auto item: *this)
                        delete item;
                }
        };



    }
}

#endif
