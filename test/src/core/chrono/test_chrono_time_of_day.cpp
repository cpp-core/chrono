// Copyright 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/chrono/time_of_day.h"
#include "core/chrono/time_of_day_stream.h"
#include "coro/stream/stream.h"
#include "core/string/lexical_cast.h"

using namespace chron;
using namespace coro;

static const int NumberSamples = 5;

TEST(TimeOfDay, Min)
{
    auto tod = TimeOfDay::min();
    EXPECT_EQ(tod.hours().count(), 0);
    EXPECT_EQ(tod.minutes().count(), 0);
    EXPECT_EQ(tod.seconds().count(), 0);
    EXPECT_EQ(tod.subseconds().count(), 0);
}

TEST(TimeOfDay, Max)
{
    auto tod = TimeOfDay::max();
    EXPECT_EQ(tod.hours().count(), 23);
    EXPECT_EQ(tod.minutes().count(), 59);
    EXPECT_EQ(tod.seconds().count(), 59);
    EXPECT_EQ(tod.subseconds().count(), 999'999'999ll);
}

TEST(TimeOfDay, ConstructBase)
{
    for (auto tod : sampler<TimeOfDay>() | take(NumberSamples)) {
	auto tod_base = (TimeOfDayBase)tod;
	auto actual = TimeOfDay{tod_base};
	EXPECT_EQ(actual, tod);
    }
}

TEST(TimeOfDay, ConstructComponents)
{
    auto g = sampler<int>(0, 23)
	* sampler<int>(0, 59)
	* sampler<int>(0, 59)
	* sampler<std::int64_t>(0ll, 999'999'999ll)
	| zip()
	| take(NumberSamples);
    for (auto [h, m, s, n] : g) {
	TimeOfDay tod{h, m, s, n};
	auto interval = n + 1'000'000'000ll * (s + 60 * (m + 60 * h));
	EXPECT_EQ(tod.to_duration().count(), interval);
    }
}

TEST(TimeOfDay, ConstructString)
{
    for (auto tod : sampler<TimeOfDay>() | take(NumberSamples)) {
	auto str = core::str::to_string(tod);
	TimeOfDay actual{str};
	EXPECT_EQ(actual, tod);
    }

    TimeOfDay t0{"0"};
    EXPECT_EQ(t0.to_duration().count(), 0);

    TimeOfDay t1{"1"};
    EXPECT_EQ(t1.to_duration(), chron::hours{1});
    
    TimeOfDay t2{"0:2"};
    EXPECT_EQ(t2.to_duration(), chron::minutes{2});
    
    TimeOfDay t3{"0:0:3"};
    EXPECT_EQ(t3.to_duration(), chron::seconds{3});
    
    TimeOfDay t4{"0:0:0.000004"};
    EXPECT_EQ(t4.to_duration(), chron::micros{4});
    
    TimeOfDay t5{"0:0:.005"};
    EXPECT_EQ(t5.to_duration(), chron::millis{5});
}

TEST(TimeOfDay, ToFromJson)
{
    for (auto tod : sampler<TimeOfDay>() | take(NumberSamples)) {
	json j = tod;
	auto actual = j.get<TimeOfDay>();
	EXPECT_EQ(actual, tod);
    }
}

TEST(TimeOfDay, LexicalCast)
{
    for (auto tod : sampler<TimeOfDay>() | take(NumberSamples)) {
	auto str = core::str::to_string(tod);
	auto actual = core::str::lexical_cast<TimeOfDay>(str);
	EXPECT_EQ(actual, tod);
    }
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
