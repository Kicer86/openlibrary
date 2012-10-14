/*
    Information about cpu
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


#ifndef OPENLIBRARY_CPUINFO_HPP
#define OPENLIBRARY_CPUINFO_HPP

#include <string>
#include <inttypes.h>

namespace OpenLibrary
{
    namespace Utils
    {
        struct __attribute__ ((visibility ("default"))) Cpu
        {
            const std::string m_manufacturer;
            const uint64_t m_features;

            bool mmx() const;
            bool sse() const;
            bool sse2() const;


            Cpu();
            virtual ~Cpu();
        };

        class __attribute__ ((visibility ("default"))) CpuInfo
        {
            public:
                virtual ~CpuInfo();

                static CpuInfo* getInstance();
                const Cpu& getInfo() const;

            private:
                CpuInfo();
                Cpu m_cpu;
        };
    }
}

#endif
