
#include <iostream>
#include <fstream>

#include "bose-nelson.hpp"
#include "output_generator.hpp"

#define MAX_SIZE 128

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
    hpp_output << "const int max_items = " << MAX_SIZE << ";" << std::endl;
    
    std::vector<int> sizes;
    
    for (int i = 0; i <= MAX_SIZE; i++)
    {
        BoseNelsonSortingNetworkGenerator::SwapInstructions instructions;
        BoseNelsonSortingNetworkGenerator generator(instructions);
        generator.generate(i);

        OutputGenerator o_generator(i);
        o_generator.generate(cpp_output, instructions);
        o_generator.generate_hpp(hpp_output, instructions);
        
        sizes.push_back(instructions.size() / 2);
        
        cpp_output << std::endl;
    }
    
    //list of tables
    hpp_output << "extern int* instructionsTable[" << MAX_SIZE + 1 << "];" << std::endl << std::endl;
    cpp_output << "int* instructionsTable[" << MAX_SIZE + 1 << "] = {";
    for (int i = 0; i <= MAX_SIZE; i++)
        cpp_output << "swap_data_" << i << ", ";
    
    cpp_output << "};" << std::endl << std::endl;    

    //list of sizes
    hpp_output << "extern int instructionsTableSize[" << MAX_SIZE + 1 << "];" << std::endl << std::endl;
    cpp_output << "int instructionsTableSize[" << MAX_SIZE + 1 << "] = {";
    for (int i = 0; i <= MAX_SIZE; i++)
        cpp_output << sizes[i] << ", ";
    
    cpp_output << "};" << std::endl << std::endl;
    
    //finish namespace
    cpp_output << "}";
    hpp_output << "}";

    cpp_output.close();
    hpp_output.close();
    
    return 0;
}

