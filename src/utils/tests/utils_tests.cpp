
#include <CppUTest/TestHarness.h>

#include "../data_ptr.hpp"

TEST_GROUP(DataPtrTest)
{
};

TEST(DataPtrTest, ShouldTakeOwnerShipOverPointerPassed)
{
    struct Test
    {
        Test(int *v): m_value(v)
        {
            *m_value = 0;
        }

        ~Test()
        {
            *m_value = 1;
        }

        int *m_value;
    };

    int m = 0;
    Test *t = new Test(&m);
    data_ptr<Test> *ptr = new data_ptr<Test>(t);
    delete ptr;

    CHECK_EQUAL(1, m);
}
