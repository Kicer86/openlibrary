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
                uint32_t eax,
                         ebx,
                         ecx,
                         edx;

                Regs32(): eax(0), ebx(0), ecx(0), edx(0) {}
            };

            void cpuid(Regs32 *regs)
            {
#ifdef __GNUC__
                asm("cpuid\n\t"
                    :"+a"(regs->eax), "=b"(regs->ebx), "=c"(regs->ecx), "=d"(regs->edx));
#else
				uint32_t &eax_r = regs->eax;
				uint32_t &ebx_r = regs->ebx;
				uint32_t &ecx_r = regs->ecx;
				uint32_t &edx_r = regs->edx;
				__asm 
				{
					mov eax, eax_r
					cpuid
					mov eax_r, eax
					mov ebx_r, ebx
					mov ecx_r, ecx
					mov edx_r, edx

				}
#endif
            }

            std::string reg32_to_str(uint32_t reg)
            {
                uint64_t ereg = reg;                                 //cast 32 bit value to 64 bit (to create 32 bits of data + zeros)
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

            uint64_t features()
            {
                Regs32 regs;
                regs.eax = 1;
                cpuid(&regs);

                const uint64_t result = (static_cast<uint64_t>(regs.ecx) << 32) | (regs.edx & 0xffffffff);

                return result;
            }
        }


        Cpu::Cpu(): m_manufacturer(manufacturer()), m_features(features())
        {

        }


        Cpu::~Cpu()
        {

        }


        bool Cpu::mmx() const
        {
            return (m_features & (1 << 23)) != 0;
        }


        bool Cpu::sse() const
        {
            return (m_features & (1 << 25)) != 0;
        }


        bool Cpu::sse2() const
        {
            return (m_features & (1 << 26)) != 0;
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


        const Cpu& CpuInfo::getInfo() const
        {
            return m_cpu;
        }


    }
}
