
#include "ts_resource.hpp"

#include <iostream>

namespace ol
{

    ThreadSafeResource<std::ostream *> ThreadSafeOutput(&std::cout);
    
}
