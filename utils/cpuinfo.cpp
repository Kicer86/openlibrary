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

#include "cpuinfo.hpp"

namespace OpenLibrary
{
    namespace Utils
    {
        namespace
        {
            struct Regs32
            {
                __uint32_t eax,
                           ebx,
                           ecx,
                           edx;

                Regs32(): eax(0), ebx(0), ecx(0), edx(0) {}
            };

            void cpuid(Regs32 *regs)
            {
                asm("cpuid\n\t"
                    :"+a"(regs->eax), "=b"(regs->ebx), "=c"(regs->ecx), "=d"(regs->edx));
            }

            std::string reg32_to_str(__uint32_t reg)
            {
                __uint64_t ereg = reg;                              //cast 32 bit value to 64 bit (to create 32 bits of data + zeros)
                std::string result = reinterpret_cast<char*>(&ereg); //4 chars + 4 zeros

                return result;
            }

            std::string manufacturer()
            {
                Regs32 regs;
                cpuid(&regs);

                const std::string result = reg32_to_str(regs.ebx) + reg32_to_str(regs.edx) + reg32_to_str(regs.ecx);

                return result;
            }
        }


        Cpu::Cpu(): m_cpu_manufacturer(manufacturer())
        {

        }


        Cpu::~Cpu()
        {

        }

        /////////////////////////////////////////////////////////////////////////////////

        CpuInfo::CpuInfo(): m_cpu()
        {


        }


        CpuInfo::~CpuInfo()
        {

        }


        CpuInfo *CpuInfo::getInstance()
        {
            static CpuInfo instance;

            return &instance;
        }

    }
}
