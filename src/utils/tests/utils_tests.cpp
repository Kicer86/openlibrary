
#include <CppUTest/TestHarness.h>

#include "../data_ptr.hpp"

TEST_GROUP(DataPtrTest)
{
};

TEST(DataPtrTest, ShouldTakeOwnerShipOverPointerPassed)
{
    struct Test
    {
        int m_dat;
    };

    Test *t = new Test;
    data_ptr<Test> ptr(t);

    //CppUTest will automaticaly check for memory leak
}


TEST(DataPtrTest, ShouldCopyDataWhenDataPtrCopied)
{
    struct Test
    {
        Test(): m_data(1) {}

        int m_data;
    };

    Test *test = new Test;

    data_ptr<Test> ptr1(test);
    data_ptr<Test> ptr2 = ptr1;

    ptr1->m_data = 5;
    ptr2->m_data = 7;

    CHECK_EQUAL(5, ptr1->m_data);
    CHECK_EQUAL(7, ptr2->m_data);
}


TEST(DataPtrTest, ShouldCopyDataWhenDataPtrAssigned)
{
    struct Test
    {
        Test(): m_data(1) {}

        int m_data;
    };

    Test *test = new Test;

    data_ptr<Test> ptr1(test);
    data_ptr<Test> ptr2;

    ptr2 = ptr1;

    ptr1->m_data = 3;
    ptr2->m_data = 8;

    CHECK_EQUAL(3, ptr1->m_data);
    CHECK_EQUAL(8, ptr2->m_data);
}


TEST(DataPtrTest, ShouldCopyDataWhenDataAssigned)
{
    struct Test
    {
        Test(): m_data(1) {}

        int m_data;
    };

    Test *test = new Test;
    test->m_data = 1;

    data_ptr<Test> ptr1;
    ptr1 = test;
    test->m_data = 4;
    ptr1->m_data = 6;

    //data_ptr takes ownership ower pointer, so both values should be equal
    CHECK_EQUAL(6, test->m_data);
    CHECK_EQUAL(6, ptr1->m_data);

    Test *test2 = new Test;
    ptr1 = test2;

    ptr1->m_data = 7;

    CHECK_EQUAL(7, ptr1->m_data);

    //here all data should be deleted
}


TEST(DataPtrTest, ShouldBehaveProperlyForDereferenceOperators)
{
    struct Test
    {
        Test(): m_data(1) {}

        int m_data;
    };

    Test *test = new Test;
    data_ptr<Test> ptr(test);

    POINTERS_EQUAL(test, &(*ptr));
    POINTERS_EQUAL(&(test->m_data), &(ptr->m_data));
}


TEST(DataPtrTest, ShouldBehaveProperlyForManyAssignOperators)
{
    struct Test
    {
        Test(): m_data(1) {}

        int m_data;
    };

    Test *test = new Test;
    test->m_data = 10;

    data_ptr<Test> ptr1;
    data_ptr<Test> ptr2;
    data_ptr<Test> ptr3;
    data_ptr<Test> ptr4;
    data_ptr<Test> ptr5;

    ptr1 = ptr2 = ptr3 = ptr4 = ptr5 = test;

    CHECK_EQUAL(10, test->m_data);
    CHECK_EQUAL(10, ptr1->m_data);
    CHECK_EQUAL(10, ptr2->m_data);
    CHECK_EQUAL(10, ptr3->m_data);
    CHECK_EQUAL(10, ptr4->m_data);
    CHECK_EQUAL(10, ptr5->m_data);

    ptr1->m_data = 11;
    ptr2->m_data = 12;
    ptr3->m_data = 13;
    ptr4->m_data = 14;
    ptr5->m_data = 15;

    CHECK_EQUAL(15, test->m_data);       //test == ptr5
    CHECK_EQUAL(11, ptr1->m_data);
    CHECK_EQUAL(12, ptr2->m_data);
    CHECK_EQUAL(13, ptr3->m_data);
    CHECK_EQUAL(14, ptr4->m_data);
    CHECK_EQUAL(15, ptr5->m_data);
}
