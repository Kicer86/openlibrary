
#ifndef STR_OP_HPP
#define STR_OP_HPP

#include <string>

#include "OpenLibraryExports.hpp"

namespace Strings
{
    OL_EXPORTS std::string *stripQuotas(std::string *s);
    OL_EXPORTS std::string stripBlanks(const std::string &s);
}

#endif
