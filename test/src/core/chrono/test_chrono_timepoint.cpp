// Copyright 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/chrono/chrono_stream.h"
#include "core/mp/foreach.h"
#include "core/string/lexical_cast.h"

using namespace chron;
using namespace coro;

static const int NumberSamples = 64;

auto tznamer() {
    return repeat("EST") * repeat("CST") * repeat("UTC") * repeat("Europe/Berlin") | choose();
}

auto nano_fuzzer() {
    return Sampler<std::int64_t>{}(0, (24 * 60 * 60 - 1) * 1'000'000'000ll);
}

TEST(TimePoint, Epoch)
{
    auto tp = TimePoint::epoch();
    EXPECT_EQ(tp.time_since_epoch().count(), 0);
}

TEST(TimePoint, Now)
{
    EXPECT_GT(TimePoint::now(), TimePoint::epoch());
}

TEST(TimePoint, Max)
{
    EXPECT_GT(TimePoint::max(), TimePoint::now());
}

TEST(TimePoint, ConstructBase)
{
    for (auto tp : Sampler<TimePoint>{}() | take(NumberSamples)) {
	TimePointBase tpb = tp;
	TimePoint actual{tpb};
	EXPECT_EQ(actual, tp);
    }
}

TEST(TimePoint, ConstructNanos)
{
    for (auto tp : Sampler<TimePoint>{}() | take(NumberSamples)) {
	TimePointBase tpb = tp;
	std::int64_t nanos = tpb.time_since_epoch().count();
	TimePoint actual{nanos};
	EXPECT_EQ(actual, tp);
    }
}

TEST(TimePoint, ConstructSeconds)
{
    for (auto tp : Sampler<TimePoint>{}() | take(NumberSamples)) {
	TimePointBase tpb = tp;
	auto seconds = 1e-9 * tpb.time_since_epoch().count();
	TimePoint actual{seconds};
	auto delta = tp - actual;
	EXPECT_LT(delta.count(), 1000);
    }
}

TEST(TimePoint, ConstructDate)
{
    auto namer = tznamer();
    for (auto date : Sampler<Date>{}() | take(NumberSamples)) {
	TimeZoneName tzname{namer.sample()};
	TimePoint tp{date, tzname};
	auto actual = tp.date(tzname);
	EXPECT_EQ(actual, date);
    }
}

TEST(TimePoint, ConstructDateTimeOfDay)
{
    auto namer = tznamer();
    auto g = Sampler<Date>{}() * Sampler<TimeOfDay>{}() | zip() | take(NumberSamples);
    for (auto [date, tod] : g) {
	TimeZoneName tzname{namer.sample()};
	TimePoint tp{date, tod, tzname};
	auto actual_date = tp.date(tzname);
	auto actual_tod = tp.time_of_day(tzname);
	EXPECT_EQ(actual_date, date);
	EXPECT_EQ(actual_tod, tod);
    }
}

TEST(TimePoint, ConstructString)
{
    auto namer = tznamer();
    for (auto tp : Sampler<TimePoint>{}() | take(NumberSamples)) {
	TimeZoneName tzname{namer.sample()};
	auto str = tp.to_string(tzname);
	auto actual = TimePoint{str, tzname};
	EXPECT_EQ(actual, tp);
    }
}

TEST(TimePoint, Date)
{
    auto namer = tznamer();
    auto fuzz = nano_fuzzer();
    for (auto date : Sampler<Date>{}() | take(NumberSamples)) {
	TimeZoneName tzname{namer.sample()};
	TimePoint tp{date, tzname};
	tp += nanos{fuzz.sample()};
	auto actual = tp.date(tzname);
	EXPECT_EQ(actual, date);
    }
}

TEST(TimePoint, TimeOfDay)
{
    auto namer = tznamer();
    for (auto [date, tod] : sampler<Date>() * sampler<TimeOfDay>() | zip() | take(NumberSamples)) {
	TimeZoneName tzname{namer.sample()};
	TimePoint tp{date, tod, tzname};
	EXPECT_EQ(tp.time_of_day(tzname), tod);
    }
}

TEST(TimePoint, Components)
{
    auto namer = tznamer();
    for (auto [date, tod] : sampler<Date>() * sampler<TimeOfDay>() | zip() | take(NumberSamples)) {
	TimeZoneName tzname{namer.sample()};
	TimePoint tp{date, tod, tzname};
	EXPECT_EQ(tp.date(tzname), date);
	EXPECT_EQ(tp.time_of_day(tzname), tod);
    }
}

TEST(TimePoint, Midnight)
{
    auto namer = tznamer();
    auto fuzz = nano_fuzzer();
    for (auto date : Sampler<Date>{}() | take(NumberSamples)) {
	TimeZoneName tzname{namer.sample()};
	TimePoint tp{date, tzname};
	
	auto tp_fuzz = tp + nanos{fuzz.sample()};
	EXPECT_EQ(tp_fuzz.midnight(tzname), tp);

	TimePoint tpx{date.tomorrow(), tzname};
	EXPECT_EQ(tp_fuzz.next_midnight(tzname), tpx);
    }
}

TEST(TimePoint, UnixTs)
{
    for (auto tp : sampler<TimePoint>() | take(NumberSamples)) {
	auto ts = tp.unix_ts();
	TimePoint tpx{ts};
	auto interval = tp - tpx;
	EXPECT_LT(interval.count(), 1'000);
    }
}

TEST(TimePoint, DurationArithmetic)
{
    core::mp::foreach<DurationAll>([]<class T>() {
	    auto durations = Sampler<T>{}();
	    for (auto tp : Sampler<TimePoint>{}() | take(NumberSamples)) {
		auto duration = durations.sample();
	
		auto tpx = tp + duration;
		::testing::StaticAssertTypeEq<decltype(tpx), TimePoint>();
		auto xdelta = tpx - tp;
		EXPECT_EQ(xdelta, duration);
		
		auto tpy = tp - duration;
		::testing::StaticAssertTypeEq<decltype(tpy), TimePoint>();
		auto ydelta = tp - tpy;
		EXPECT_EQ(ydelta, duration);
	    }
	});
}

TEST(TimePoint, ToFromJson)
{
    for (auto tp : sampler<TimePoint>() | take(NumberSamples)) {
	json j = tp;
	auto actual = j.get<TimePoint>();
	EXPECT_EQ(actual, tp);
    }
}

TEST(TimePoint, LexicalCast)
{
    for (auto tp : sampler<TimePoint>() | take(NumberSamples)) {
	auto str = core::str::to_string(tp);
	auto actual = core::str::lexical_cast<TimePoint>(str);
	EXPECT_EQ(actual, tp);
    }
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
