
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
        struct Point
        {
	    int x, y;                 //coordinates
	    double f_score, g_score;  //scores
	    
	    Point(int _x, int _y): x(_x), y(_y) {}
	    Point(Point &&) = default;
	    Point &operator=(Point &&) = default;
	    
	    virtual ~Point() {}
        };
    }
}

#endif
