
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

    std::thread t1( [&]
    {
        {
            *res.lock() = 2;
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

        bool locked = res.is_locked();
        EXPECT_EQ(true, locked);

        state = 3;                            //state #3 - tell thread #1 to unlock

        while(state == 3);

        locked = res.is_locked();
        EXPECT_EQ(false, locked);
    });

}
