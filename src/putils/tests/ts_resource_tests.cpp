
#include <thread>
#include <condition_variable>
#include <atomic>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../ts_resource.hpp"

TEST(TSResourceTest, CanBeConstructed)
{
    EXPECT_NO_THROW(
    {
        ol::ThreadSafeResource<int> r1(5);
    });
}


TEST(TSResourceTest, LocksResourceForOneThread)
{
    ol::ThreadSafeResource<int> res(1);
    std::atomic<int> state(0);
    std::atomic<bool> result1(false);
    std::atomic<bool> result2(false);

    std::thread t1( [&]
    {
        {
            auto resource = res.lock();
            *resource = 2;
            state = 1;                        //state #1 - res locked and value of 2 assigned

            while(state == 1);
            while(state == 2);
        }

        state = 4;                            //state #4 - resource unlocked
    });

    std::thread t2( [&]
    {
        while(state == 0);                    //wait for state #1

        state = 2;                            //state #2 - trying to lock by second thread

        result1 = res.is_locked();

        state = 3;                            //state #3 - tell thread #1 to unlock

        while(state == 3);

        result2 = res.is_locked();
    });

    t1.join();
    t2.join();

    EXPECT_EQ(true, result1);
    EXPECT_EQ(false, result2);
    EXPECT_EQ(2, *res.lock());
}
