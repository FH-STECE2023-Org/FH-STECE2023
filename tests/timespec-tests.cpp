#include <door/utilities/timespec.h>

#include <gtest/gtest.h>
#include <time.h>


TEST(timespec_suite, default_ctor)
{
    TimeSpec fancy;
    ASSERT_EQ(fancy.tv_sec, 0);
    ASSERT_EQ(fancy.tv_nsec, 0);
}

TEST(timespec_suite, ctor_from_good_old)
{
    timespec good_old = {
        .tv_sec = 100,
        .tv_nsec = 550,
    };

    TimeSpec fancy_new = good_old;                     // <-- constructor
    ASSERT_EQ(fancy_new.tv_sec, 100);
    ASSERT_EQ(fancy_new.tv_nsec, 550);
}

TEST(timespec_suite, explicit_ctor)
{
    TimeSpec t(100, 550);
    ASSERT_EQ(t.tv_sec, 100);
    ASSERT_EQ(t.tv_nsec, 550);
}

TEST(timespec_suite, copy_from_good_old)
{
    timespec good_old = {
        .tv_sec = 100,
        .tv_nsec = 550,
    };

    TimeSpec fancy_new;                                // <-- default-constructor
    fancy_new = good_old;                              // <-- copy

    ASSERT_EQ(fancy_new.tv_sec, 100);
    ASSERT_EQ(fancy_new.tv_nsec, 550);
}

TEST(timespec_suite, copy_ctor)
{
    TimeSpec orig(100, 550);    
    TimeSpec copy(orig);                               // <-- copy constructor

    ASSERT_EQ(copy.tv_sec, 100);
    ASSERT_EQ(copy.tv_nsec, 550);
}

TEST(timespec_suite, assignment_operator)
{
    TimeSpec orig(100, 550);
    TimeSpec copy;                                     // <-- default constructor

    copy = orig;                                       // <-- assignment operator

    ASSERT_EQ(copy.tv_sec, 100);
    ASSERT_EQ(copy.tv_nsec, 550);
}

TEST(timespec_suite, less)
{
    TimeSpec older(100, 550);

    // TimeSpec < TimeSpec
    {
        TimeSpec newer1(100, 551);
        TimeSpec newer2(101, 550);
        TimeSpec newer3(101, 551);

        ASSERT_TRUE(older < newer1);
        ASSERT_TRUE(older < newer2);
        ASSERT_TRUE(older < newer3);
    }

    // TimeSpec < (good old) timespec
    {
        timespec newer1 = { .tv_sec = 100, .tv_nsec = 551 };
        timespec newer2 = { .tv_sec = 101, .tv_nsec = 550 };
        timespec newer3 = { .tv_sec = 101, .tv_nsec = 551 };

        ASSERT_TRUE(older < newer1);
        ASSERT_TRUE(older < newer2);
        ASSERT_TRUE(older < newer3);
    }
}

TEST(timespec_suite, equals_and_notequals)
{
    ASSERT_TRUE(TimeSpec(100, 550) == TimeSpec(100, 550));
    ASSERT_FALSE(TimeSpec(100, 551) == TimeSpec(100, 550));
    ASSERT_FALSE(TimeSpec(101, 550) == TimeSpec(100, 550));

    ASSERT_FALSE(TimeSpec(100, 550) != TimeSpec(100, 550));
    ASSERT_TRUE(TimeSpec(100, 551) != TimeSpec(100, 550));
    ASSERT_TRUE(TimeSpec(101, 550) != TimeSpec(100, 550));
}

TEST(timespec_suite, greater)
{
    ASSERT_TRUE(TimeSpec(101, 550) > TimeSpec(100, 550));
    ASSERT_TRUE(TimeSpec(100, 551) > TimeSpec(100, 550));
}

TEST(timespec_suite, less_equal)
{
    ASSERT_TRUE(TimeSpec(100, 550) <= TimeSpec(100, 550));
    ASSERT_TRUE(TimeSpec(100, 550) <= TimeSpec(100, 551));
    ASSERT_TRUE(TimeSpec(100, 550) <= TimeSpec(101, 550));
}

TEST(timespec_suite, greater_equal)
{
    ASSERT_TRUE(TimeSpec(100, 550) >= TimeSpec(100, 550));
    ASSERT_TRUE(TimeSpec(100, 551) >= TimeSpec(100, 550));
    ASSERT_TRUE(TimeSpec(101, 550) >= TimeSpec(100, 550));
}

TEST(timespec_suite, now_monotonic)
{
    [[maybe_unused]] TimeSpec now = TimeSpec::now_monotonic();
    // hard to test which time it returned :-)
}

TEST(timespec_suite, from_milliseconds)
{
    TimeSpec t = TimeSpec::from_milliseconds(1);
    ASSERT_EQ(t, TimeSpec(0, 1000*1000));
}

TEST(timespec_suite, add)
{
    // within nanoseconds range
    ASSERT_EQ(TimeSpec(100, 550) + TimeSpec(0, 1),
              TimeSpec(100, 551));

    // nanoseconds overflow into seconds
    ASSERT_EQ(TimeSpec(100, 1000*1000*1000 - 1) + TimeSpec(0, 1),
              TimeSpec(101, 0));
}

TEST(timespec_suite, sub)
{
    // within nanoseconds range
    ASSERT_EQ(TimeSpec(100, 550) - TimeSpec(0, 1), 
              TimeSpec(100, 549));
    
    // nanoseconds underflow into seconds
    ASSERT_EQ(TimeSpec(100, 550) - TimeSpec(0, 551),
              TimeSpec(99, 1000*1000*1000 - 1));
}
