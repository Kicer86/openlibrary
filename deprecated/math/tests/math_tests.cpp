
#include <CppUTest/TestHarness.h>

#include "math/math.hpp"

TEST_GROUP(MathTest)
{



};


TEST(MathTest, unsigned32bitVariableShouldBehaveAsUnsigned32bit)
{
    OpenLibrary::Math::BitType<32, false>::type u32_var = 0xffffffff;

    u32_var++;
    CHECK_EQUAL(0, u32_var);

    u32_var = 0x80000000 - 1;
    u32_var++;
    CHECK_EQUAL(0x80000000, u32_var);
}
