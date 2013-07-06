
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
    
    boost::optional<int> i1 = queue.pop_front();
    boost::optional<int> i2 = queue.pop_front();
    
    CHECK_EQUAL(false, !i1);
    CHECK_EQUAL(false, !i2);
    
    CHECK_EQUAL(1, i1);
    CHECK_EQUAL(2, i2);
}


TEST(TS_QueueShould, DealWithTakingFromEmptyContainer)
{
    TS_Queue<std::deque<int>> queue(10);
    
    queue.push_back(1);
    
    boost::optional<int> i1 = queue.pop_front();
    
    i1 = queue.pop_front(std::chrono::duration<int>::min());
        
    CHECK_EQUAL(true, !i1);
}
