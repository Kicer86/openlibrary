/*
    Mathematical routines
    Copyright (C) 2012  Michał Walenciak Kicer86 at gmail.co

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


#ifndef OPENLIBRARY_MATH_HPP
#define OPENLIBRARY_MATH_HPP

#include <sys/types.h>

__int16_t dffd;

namespace OpenLibrary
{
    namespace Math
    {

        template<int size = 32, bool sign = true>
        struct BitType
        {
            typedef void type;
        };

        template<>
        struct BitType<16, true>
        {
            typedef __int16_t type;
        };

        template<>
        struct BitType<16, false>
        {
            typedef __uint16_t type;
        };

        template<>
        struct BitType<32, true>
        {
            typedef __int32_t type;
        };

        template<>
        struct BitType<32, false>
        {
            typedef __uint32_t type;
        };

        template<>
        struct BitType<64, true>
        {
            typedef __int64_t type;
        };

        template<>
        struct BitType<64, false>
        {
            typedef __uint64_t type;
        };

    }
}

#endif
