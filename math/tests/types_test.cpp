
#include "../math.hpp"

using namespace OpenLibrary::Math;

#define EQUALS(a, b, ec) if (a != b) return ec;

int main()
{
    OpenLibrary::Math::BitType<32, false>::type u32_var = 0xffffffff;

    EQUALS(u32_var, 0xffffffff, 1);         //truly at least 32bit?
    u32_var++;
    EQUALS(u32_var, 0, 2);                  //truly at most 32bit?



    return 0;
}
