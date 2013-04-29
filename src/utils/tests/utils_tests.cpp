
#include <CppUTest/TestHarness.h>
#include <memory>

#include "../data_ptr.hpp"

//access to private data... ugly but works ;)
#define private public
#include "../anonymous_ptr.hpp"

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


TEST(DataPtrTest, ShouldHandleAssigningItself)
{
    struct Test
    {
        Test(): m_data(1) {}

        int m_data;
    };

    Test *test = new Test;

    data_ptr<Test> ptr1(test);

    ptr1 = ptr1;
    ptr1->m_data = 2;

    CHECK_EQUAL(2, test->m_data);
}


TEST(DataPtrTest, ShouldHandleAssigningTheSamePtrManyTimes)
{
    struct Test
    {
        Test(): m_data(1) {}

        int m_data;
    };

    Test *test = new Test;

    data_ptr<Test> ptr1(test);
    ptr1 = test;
    ptr1 = test;

    ptr1->m_data = 2;

    CHECK_EQUAL(2, test->m_data);
}


/******************************************************************************/


TEST_GROUP(AnonymousPtrTest)
{
    
};

TEST(AnonymousPtrTest, ShouldCallDeleterWhenDeleted)
{
    bool done = false;
    
    struct Deleter
    {
        Deleter(bool *done): m_done(done) {}
        
        void deinit()
        {
            *m_done = true;
        }
        
        bool *m_done;
    } deleter(&done);

    int d;
    anonymous_ptr<int, Deleter> *ptr = new anonymous_ptr<int, Deleter>(&d, &deleter);
    
    delete ptr;

    CHECK_EQUAL(true, done);
}


TEST(AnonymousPtrTest, ShouldHasRefCountEqualToOneWhenConstructed)
{    
    struct Deleter
    {
        Deleter() {}        
        void deinit() {}
        
    } deleter;

    int d;
    anonymous_ptr<int, Deleter> *ptr = new anonymous_ptr<int, Deleter>(&d, &deleter);
    
    CHECK_EQUAL(1, ptr->m_data->m_ref_count);
    
    delete ptr;
}


TEST(AnonymousPtrTest, ShouldIncreaseRefCountWhenCopied)
{    
    struct Deleter
    {
        Deleter() {}        
        void deinit() {}
        
    } deleter;

    int d;
    anonymous_ptr<int, Deleter> ptr1 (&d, &deleter);
    
    CHECK_EQUAL(1, ptr1.m_data->m_ref_count);
    
    anonymous_ptr<int, Deleter> ptr2(ptr1);
        
    CHECK_EQUAL(2, ptr1.m_data->m_ref_count);
    CHECK_EQUAL(2, ptr2.m_data->m_ref_count);
    
    anonymous_ptr<int, Deleter> ptr3(ptr2);
    
    CHECK_EQUAL(3, ptr1.m_data->m_ref_count);
    CHECK_EQUAL(3, ptr2.m_data->m_ref_count);
    CHECK_EQUAL(3, ptr3.m_data->m_ref_count);
    
    anonymous_ptr<int, Deleter> ptr4(ptr1);
    
    CHECK_EQUAL(4, ptr1.m_data->m_ref_count);
    CHECK_EQUAL(4, ptr2.m_data->m_ref_count);
    CHECK_EQUAL(4, ptr3.m_data->m_ref_count);
    CHECK_EQUAL(4, ptr4.m_data->m_ref_count);
}


TEST(AnonymousPtrTest, ShouldSharePrivateData)
{    
    struct Deleter
    {
        Deleter() {}        
        void deinit() {}
        
    } deleter;

    int d;
    anonymous_ptr<int, Deleter> ptr (&d, &deleter);
    anonymous_ptr<int, Deleter> ptr2(ptr);
    anonymous_ptr<int, Deleter> ptr3(ptr2);
    anonymous_ptr<int, Deleter> ptr4(ptr);
    
    CHECK_EQUAL(ptr.m_data, ptr2.m_data);
    CHECK_EQUAL(ptr.m_data, ptr3.m_data);
    CHECK_EQUAL(ptr.m_data, ptr4.m_data);
    
    CHECK_EQUAL(ptr2.m_data, ptr3.m_data);
    CHECK_EQUAL(ptr.m_data, ptr4.m_data);
    
    CHECK_EQUAL(ptr3.m_data, ptr4.m_data);    
}


TEST(AnonymousPtrTest, ShouldDecreaseRefCountWhenDestroyed)
{    
    struct Deleter
    {
        Deleter() {}        
        void deinit() {}
        
    } deleter;

    int d;
    anonymous_ptr<int, Deleter> ptr (&d, &deleter);
    anonymous_ptr<int, Deleter> *ptr2 = new anonymous_ptr<int, Deleter>(ptr);
    
    CHECK_EQUAL(2, ptr.m_data->m_ref_count);
    
    delete ptr2;
    
    CHECK_EQUAL(1, ptr.m_data->m_ref_count);
}


TEST(AnonymousPtrTest, ShouldDropPreviousDataWhenReseted)
{    
    struct Deleter
    {
        Deleter(): m_d(false) {}        
        void deinit() {m_d = true; }
        
        bool m_d;
        
    } deleter;

    int d;
    anonymous_ptr<int, Deleter> ptr (&d, &deleter);
    
    CHECK_EQUAL(false, deleter.m_d);
    
    int e;
    ptr.reset(&e);
    
    CHECK_EQUAL(true, deleter.m_d);
    
    deleter.m_d = false;
    
    ptr.reset(nullptr);
    
    CHECK_EQUAL(true, deleter.m_d);    
}


/******************************************************************************/


TEST_GROUP(AnonymousUniqPtrTest)
{
    
};


TEST(AnonymousUniqPtrTest, ShouldCallDeleterWhenDeleted)
{
    bool done = false;
    
    struct Deleter
    {
        Deleter(bool *done): m_done(done) {}
        
        void deinit()
        {
            *m_done = true;
        }
        
        bool *m_done;
    } deleter(&done);

    int d;
    anonymous_uniq_ptr<int, Deleter> *ptr = new anonymous_uniq_ptr<int, Deleter>(&d, &deleter);
    
    delete ptr;

    CHECK_EQUAL(true, done);
}


TEST(AnonymousUniqPtrTest, ShouldCallDeleterWhenResetedToNewValue)
{
    struct Deleter
    {
        Deleter(): m_d(false) {}        
        void deinit() {m_d = true; }
        
        bool m_d;
        
    } deleter;

    int d;
    anonymous_uniq_ptr<int, Deleter> ptr (&d, &deleter);
    
    CHECK_EQUAL(false, deleter.m_d);
    
    int e;
    ptr.reset(&e);
    
    CHECK_EQUAL(true, deleter.m_d);
    
    deleter.m_d = false;
    
    ptr.reset(nullptr);
    
    CHECK_EQUAL(true, deleter.m_d);    
}


TEST(AnonymousUniqPtrTest, ShouldNotCallDeleterWhileMoving)
{
    struct Deleter
    {
        Deleter(): m_d(false) {}        
        void deinit() {m_d = true; }
        
        bool m_d;
        
    } deleter;

    //initial conditions
    int d;
    anonymous_uniq_ptr<int, Deleter> *ptr = new anonymous_uniq_ptr<int, Deleter>(&d, &deleter);
    
    CHECK_EQUAL(false, deleter.m_d);
    
    //do move
    anonymous_uniq_ptr<int, Deleter> *ptr2 = new anonymous_uniq_ptr<int, Deleter>( std::move(*ptr));
    delete ptr;
      
    //deleter should not be called yet - we moved data
    CHECK_EQUAL(false, deleter.m_d);
    
    //final destruction
    delete ptr2;
    
    //deleter should be called
    CHECK_EQUAL(true, deleter.m_d);
}


TEST(AnonymousUniqPtrTest, ShouldAllowToReturnEncapsulatedPointer)
{
    struct Deleter
    {
        Deleter() {}
        void deinit() {}                
    } deleter;

    //initial conditions
    int d;
    anonymous_uniq_ptr<int, Deleter> ptr(&d, &deleter);
    
    CHECK_EQUAL(&d, ptr.get());

    int e;
    ptr.reset(&e);
    
    CHECK_EQUAL(&e, ptr.get());
}
