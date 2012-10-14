/*
    CRC32 algorithm implementation
    Copyright (C) 2012  Michał Walenciak <Kicer86 at gmail.com>

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


#ifndef OPENLIBRARY_CRC32_HPP
#define OPENLIBRARY_CRC32_HPP

#include <inttypes.h>

namespace OpenLibrary
{

    namespace Math
    {
        struct Crc32Impl;

        class Crc32
        {

            public:
                Crc32();
                virtual ~Crc32();

                uint32_t calculate(void *, int len) const;

            private:
                Crc32Impl *m_impl;
        };
    }
}

#endif
