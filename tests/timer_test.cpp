#include <door/utilities/one-shot-timer.h>
#include <gtest/gtest.h>
#include <stdbool.h>
#include <stdlib.h>

TEST(timer_suite, timer_running)
{
    TimeSpec set_time(1, 0);

    OneShotTimer one_shot(set_time,[]()
        {std::cout << "Oneshot Timer created" << std::endl;});
    
    one_shot.start();
    ASSERT_EQ(one_shot.isrunning(), true);

    one_shot.stop();
    ASSERT_EQ(one_shot.isrunning(), false);
}