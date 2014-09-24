
#include "ts_resource.hpp"

#include <iostream>

namespace OL_NAMESPACE
{

    ThreadSafeResource<std::ostream *> ThreadSafeOutput(&std::cout);
    
}
