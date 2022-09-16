// Copyright 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/chrono/lowres_clock.h"

using namespace chron;

TEST(LowResClock, RealTime)
{
    LowResClock clock{LowResClock::Mode::RealTime, 1us};
    EXPECT_EQ(clock.resolution(), 1us);
    auto t0 = clock.virtual_now();
    std::this_thread::sleep_for(10us);
    auto t1 = clock.virtual_now();
    EXPECT_GT(t1, t0);
    EXPECT_EQ(clock.virtual_now(), clock.now());
}

TEST(LowResClock, Virtual)
{
    LowResClock clock{LowResClock::Mode::Virtual, 1us};
    EXPECT_EQ(clock.resolution(), 1us);
    auto t0 = clock.virtual_now();
    std::this_thread::sleep_for(10us);
    auto t1 = clock.virtual_now();
    EXPECT_EQ(t1, t0);
    EXPECT_NE(clock.virtual_now(), clock.now());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
