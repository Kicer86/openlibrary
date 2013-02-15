
#ifndef STR_OP_HPP
#define STR_OP_HPP

#include <string>

#include "common_exports.hpp"

namespace Strings
{
    COMMON_EXPORTS std::string *stripQuotas(std::string *s);
    COMMON_EXPORTS std::string stripBlanks(const std::string &s);
}

#endif
