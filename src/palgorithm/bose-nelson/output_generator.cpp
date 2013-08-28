/*
    File generator for Bose-Nelson sorting networks
    Copyright (C) 2013  Michał Walenciak <MichalWalenciak@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include "output_generator.hpp"

#include <iterator>

OutputGenerator::OutputGenerator(int size): m_size(size)
{

}


void OutputGenerator::generate(std::ostream &stream, BoseNelsonSortingNetworkGenerator::SwapInstructions &instructions)
{
    generateVariable(stream, instructions);
    stream << " = ";
    generateContent(stream, instructions);
}


void OutputGenerator::generate_hpp(std::ostream &stream, BoseNelsonSortingNetworkGenerator::SwapInstructions &instructions)
{
    const int data_size = instructions.size();
    stream << "extern int swap_data_" << m_size << "[];" << std::endl;
}


void OutputGenerator::generateVariable(std::ostream &stream, BoseNelsonSortingNetworkGenerator::SwapInstructions &instructions)
{
    const int data_size = instructions.size();
    stream << "int swap_data_" << m_size << "[" << data_size << "]";
}


void OutputGenerator::generateContent(std::ostream &stream, BoseNelsonSortingNetworkGenerator::SwapInstructions &instructions)
{
    stream << "{ ";
    
    std::ostream_iterator<int> inserter (stream, ", ");
    std::copy(instructions.begin(), instructions.end(), inserter);
    
    stream << "};" << std::endl;
}
