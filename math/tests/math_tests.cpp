
#include <CppUTest/TestHarness.h>

#include "math/math.hpp"

TEST_GROUP(MathTest)
{



};


TEST(MathTest, _32bitVariableShouldbe32bit)
{
    OpenLibrary::Math::BitType<32, false>::type u32_var = 0xffffffff;

    u32_var++;

    CHECK_EQUAL(-1, u32_var);
}
