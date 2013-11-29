
#include <iostream>

#include <dlfcn.h>

#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << std::endl;
        std::cout << argv[0] << "/path/to/library/with/tests.so" << std::endl;
        
        return 1;
    }
    
    //load library
    dlopen(argv[1], RTLD_LAZY);
    
    argc--;  //remove first argument and
    argv++;  //move to second one
   
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
