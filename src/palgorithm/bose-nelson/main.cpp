
#include <iostream>
#include <fstream>

#include "bose-nelson.hpp"
#include "output_generator.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
        return 1;
    
    const std::string cpp_file = argv[1];
    const std::string hpp_file = argv[2];
    
    std::fstream cpp_output(cpp_file, std::ios_base::out);
    std::fstream hpp_output(hpp_file, std::ios_base::out);
    
    cpp_output << "namespace BoseNelson {" << std::endl;
    hpp_output << "namespace BoseNelson {" << std::endl;
    
    for (int i = 0; i <= 128; i++)
    {
        BoseNelsonSortingNetworkGenerator::SwapInstructions instructions;
        BoseNelsonSortingNetworkGenerator generator(instructions);
        generator.generate(i);

        OutputGenerator o_generator(i);
        o_generator.generate(cpp_output, instructions);
        o_generator.generate_hpp(hpp_output, instructions);
        
        cpp_output << std::endl;
    }
    
    cpp_output << "}";
    hpp_output << "}";

    cpp_output.close();
    hpp_output.close();
    
    return 0;
}

