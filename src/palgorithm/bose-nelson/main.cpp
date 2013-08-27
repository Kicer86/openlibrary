
#include <iostream>

#include "bose-nelson.hpp"
#include "output_generator.hpp"

int main()
{
    BoseNelsonSortingNetworkGenerator::SwapInstructions instructions;
    BoseNelsonSortingNetworkGenerator generator(instructions);
    
    generator.generate(32);

    OutputGenerator o_generator(32);
    
    o_generator.generate(std::cout, instructions);
    
    return 0;
}
