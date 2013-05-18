
#include <CppUTest/TestHarness.h>
#include <memory>
#include <deque>

#include "../ts_queue.hpp"

TEST_GROUP(TS_QueueShould)
{
};

TEST(TS_QueueShould, ReturnAllPassedData)
{
    TS_Queue<std::deque<int>> queue(10);
    
    queue.push_back(1);
    queue.push_back(2);
    
    const int i1 = queue.pop_front();
    const int i2 = queue.pop_front();
    
    CHECK_EQUAL(1, i1);
    CHECK_EQUAL(2, i2);
}
