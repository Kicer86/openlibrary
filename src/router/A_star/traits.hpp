
/***************************************
 *           Various traits            *
 *      Author: Michał Walenciak       *
 *      Creation date: 29.09.2012      *
 ***************************************/

#ifndef OPENLIBRARY_A_START_ROUTER_TRAITS_HPP
#define OPENLIBRARY_A_START_ROUTER_TRAITS_HPP

#include <forward_list>
#include <list>
#include <deque>

#ifdef _MSC_VER
	#define constexpr const
#endif

namespace ol
{
    namespace Router
    {
        template<class T>
        struct Container
        {
            static constexpr bool prependable = false;
        };


        //template<>
        template<class U>
        struct Container< std::deque<U> >
        {
            static constexpr bool prependable = true;
        };


        //template<>
        template<class U>
        struct Container< std::list<U> >
        {
            static constexpr bool prependable = true;
        };


        //template<>
        template<class U>
        struct Container< std::forward_list<U> >
        {
            static constexpr bool prependable = true;
        };
    }
}

#endif
