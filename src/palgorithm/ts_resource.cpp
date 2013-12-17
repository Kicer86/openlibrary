
#include "ts_resource.hpp"

#include <iostream>

ThreadSafeResource<std::ostream *> ThreadSafeOut(&std::cout);
