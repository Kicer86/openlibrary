
#ifndef STR_OP_HPP
#define STR_OP_HPP

#include <string>

#include "common_export.h"

namespace Strings
{
    COMMON_EXPORT std::string *stripQuotas(std::string *s);
    COMMON_EXPORT std::string stripBlanks(const std::string &s);
}

#endif
