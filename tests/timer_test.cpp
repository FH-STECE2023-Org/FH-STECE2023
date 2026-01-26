#include <door/utilities/one-shot-timer.h>
#include <gtest/gtest.h>
#include <stdbool.h>
#include <stdlib.h>

TEST(timer_suite, timer_running)
{
    //1s time
    TimeSpec set_time(1, 0);

    OneShotTimer one_shot(set_time,[]()
        {std::cout << "Oneshot Timer created" << std::endl;});
    
    one_shot.start();
    ASSERT_EQ(one_shot.isrunning(), true);

    //Stop timer otherwise you get an error in the destructor of the timer object
    one_shot.stop();
    ASSERT_EQ(one_shot.isrunning(), false);  
}

TEST(timer_suite, timer_start_twice)
{
    //1s time
    TimeSpec set_time(1, 0);

    OneShotTimer one_shot(set_time,[]()
        {std::cout << "Oneshot Timer created" << std::endl;});
    
    one_shot.start();
    ASSERT_EQ(one_shot.isrunning(), true);

    one_shot.start();
    ASSERT_EQ(one_shot.isrunning(), true);  

    one_shot.stop();
}

TEST(timer_suite, timer_stop_twice)
{
    //1s time
    TimeSpec set_time(1, 0);

    OneShotTimer one_shot(set_time,[]()
        {std::cout << "Oneshot Timer created" << std::endl;});
    
    one_shot.start();

    one_shot.stop();
    ASSERT_EQ(one_shot.isrunning(), false);  

    one_shot.stop();
    ASSERT_EQ(one_shot.isrunning(), false);  
}

TEST(timer_suite, expire)
{
    Eventloop loop;

    bool expired = false;
    OneShotTimer one_shot(TimeSpec(0,1),[&expired](){expired = true;});

    one_shot.hookup(loop);

    one_shot.start();

    unsigned int nloops = 10000000;
    while (!expired && nloops > 0) {
        loop.run_one();
        --nloops;
    }

    one_shot.stop();  // timer must not be running when dtor is called

    if (nloops == 0)
        FAIL();
}
