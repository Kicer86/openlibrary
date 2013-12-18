
#include "ts_resource.hpp"

#include <iostream>

ThreadSafeResource<std::ostream *> ThreadSafeOutput(&std::cout);
