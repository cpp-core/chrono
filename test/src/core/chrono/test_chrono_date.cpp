// Copyright 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/chrono/date.h"
#include "core/chrono/date_stream.h"
#include "core/chrono/timepoint.h"
#include "core/string/lexical_cast.h"
#include "coro/stream/stream.h"

using namespace chron;
using namespace coro;

static const int NumberSamples = 5;

auto tznamer() {
    return repeat("EST") * repeat("CST") * repeat("UTC") * repeat("Europe/Berlin") | choose();
}

auto nano_fuzzer() {
    return sampler<std::int64_t>(0, (24 * 60 * 60 - 1) * 1'000'000'000ll);
}

TEST(Date, fmt)
{
    Date date = aug/11/2009;
    auto s = fmt::format("{}", date);
    EXPECT_EQ(s, "2009-08-11");
}

TEST(Date, Min)
{
    auto date = Date::min();
    EXPECT_LT(date, jan/1/0000);
}

TEST(Date, Max)
{
    auto date = Date::max();
    EXPECT_GT(date, dec/31/9999);
}

TEST(Date, ConstructBase)
{
    for (auto date : sampler<Date>() | take(NumberSamples)) {
	date::year_month_day ymd{date.year(), date.month(), date.day()};
	Date actual{ymd};
	EXPECT_EQ(actual, date);
    }
}

TEST(Date, ConstructYMD)
{
    for (auto date : sampler<Date>() | take(NumberSamples)) {
	Date actual{date.year(), date.month(), date.day()};
	EXPECT_EQ(actual, date);
    }
}

TEST(Date, ConstructNanos)
{
    auto fuzz = sampler<std::int64_t>(0, (24 * 60 * 60 - 1) * 1'000'000'000ll);
    for (auto date : sampler<Date>() | take(NumberSamples)) {
	TimePoint tp = date.to_timepoint();
	std::int64_t nanos = tp.time_since_epoch().count() + fuzz.sample();
	Date actual{nanos};
	EXPECT_EQ(actual, date);
    }
}

TEST(Date, ConstructSeconds)
{
    auto fuzz = sampler<std::int64_t>(0, 24 * 60 * 60 - 1);
    for (auto date : sampler<Date>() | take(NumberSamples)) {
	TimePoint tp = date.to_timepoint();
	auto seconds = 1e-9 * tp.time_since_epoch().count() + fuzz.sample();
	Date actual{seconds};
	EXPECT_EQ(actual, date);
    }
}


TEST(Date, ConstructString)
{
    for (auto date : sampler<Date>() | take(NumberSamples)) {
	auto str = fmt::format("{:04d}-{:02d}-{:02d}",
			       (int)date.year(),
			       (uint)date.month(),
			       (uint)date.day());
	Date actual{str};
	EXPECT_EQ(actual, date);
    }
}

TEST(Date, ToTimePoint)
{
    auto namer = tznamer();
    auto fuzz = nano_fuzzer();
    for (auto date : sampler<Date>() | take(NumberSamples)) {
	TimeZoneName tzname{namer.sample()};
	auto tp = date.to_timepoint(tzname);
	tp += std::chrono::nanoseconds{fuzz.sample()};
	
	auto actual = tp.date(tzname);
	EXPECT_EQ(actual, date);
    }
}

TEST(Date, UnixTs)
{
    auto fuzzer = nano_fuzzer();
    for (auto date : sampler<Date>() | take(NumberSamples)) {
	auto ts = date.unix_ts() + 1e-9 * fuzzer.sample();
	Date actual{ts};
	EXPECT_EQ(actual, date);
    }
}

TEST(Date, TomorrowYesterday)
{
    for (auto date : sampler<Date>() | take(NumberSamples)) {
	auto tomorrow = date.tomorrow();
	auto yesterday = date.yesterday();
	EXPECT_EQ(tomorrow - yesterday, days{2});
    }
}

TEST(Date, IncrDecr)
{
    for (auto date : sampler<Date>() | take(NumberSamples)) {
	auto tomorrow = date;
	++tomorrow;
	EXPECT_EQ((tomorrow - date).count(), 1);

	auto yesterday = date;
	--yesterday;
	EXPECT_EQ((date - yesterday).count(), 1);

	--tomorrow;
	++yesterday;
	EXPECT_EQ(tomorrow, date);
	EXPECT_EQ(yesterday, date);
    }
}

TEST(Date, AddSubDuration)
{
    auto deltas = sampler<int>(-200, +200);
    for (auto date : sampler<Date>() | take(NumberSamples)) {
	auto delta = deltas.sample();
	auto future = date;
	future += std::chrono::days{delta};
	EXPECT_EQ((future - date).count(), delta);

	auto past = date;
	past -= std::chrono::days{delta};
	EXPECT_EQ((date - past).count(), delta);
    }
}

TEST(Date, ToFromJson)
{
    for (auto date : sampler<Date>() | take(NumberSamples)) {
	json j = date;
	auto actual = j.get<Date>();
	EXPECT_EQ(actual, date);
    }
}

TEST(Date, LexicalCast)
{
    for (auto date : sampler<Date>() | take(NumberSamples)) {
	auto str = core::str::to_string(date);
	auto actual = core::str::lexical_cast<Date>(str);
	EXPECT_EQ(actual, date);
    }
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
