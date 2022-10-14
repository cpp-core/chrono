// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include <chrono>
#include <compare>
#include <fmt/ostream.h>
#include "core/chrono/date.h"
#include "core/chrono/duration.h"
#include "core/chrono/time_of_day.h"

namespace core::chrono {

namespace literals {
using std::literals::chrono_literals::operator""d;
using std::literals::chrono_literals::operator""y;
using std::literals::chrono_literals::operator""h;
using std::literals::chrono_literals::operator""min;
using std::literals::chrono_literals::operator""s;
using std::literals::chrono_literals::operator""ms;
using std::literals::chrono_literals::operator""us;
using std::literals::chrono_literals::operator""ns;
}; // literals

using TimePointBase = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;
class Date;

// The TimePoint class 
//
class TimePoint : public TimePointBase {
public:
    using TimePointBase::TimePointBase;

    // Return the **TimePoint** for the epoch.
    static TimePoint epoch();
    
    // Return the **TimePoint** for the current time.
    static TimePoint now();
    
    // Return the **TimePoint** for the furthest representable point in the future.
    static TimePoint max();

    // Construct a **TimePoint** from the base class.
    TimePoint(const TimePointBase& base);

    // Construct a **TimePoint** `nanos` nanoseconds after the epoch.
    explicit TimePoint(std::int64_t nanos = 0);

    // Construct a **TimePoint** `ts` seconds after the epoch.
    explicit TimePoint(double ts);

    // Construct a **TimePoint** for midnight for the supplied `date` and `tzname`.
    explicit TimePoint(const Date& date, const TimeZoneName& tzname = TimeZoneName{});

    // Construct a **TimePoint** for the supplied `date`, `time_of_day` and `tzname`.
    explicit TimePoint(const Date& date, const TimeOfDay& tod,
		       const TimeZoneName& tzname = TimeZoneName{});

    // Construct a date fromm the given `str`, `fmt` and timezone `tzname`.
    TimePoint(const std::string& str,
	      const TimeZoneName& tzname = TimeZoneName{},
	      const std::string& fmt = "");

    // Return the std::string representation for this **TimePoint** using the supplied format
    // `fmt` for the given timezone `tzname`.
    std::string to_string(const TimeZoneName& tzname = TimeZoneName{},
			  const std::string& fmt = "%F %T") const;

    // Return the **Date** corresponding to this **TimePoint** for the given timezone
    // `tzname`.
    Date date(const TimeZoneName& tzname = TimeZoneName{}) const;

    // Return the **TimeOfDay** corresponding to this **TimePoint**
    // for the given timezone `tzname`.
    TimeOfDay time_of_day(const TimeZoneName& tzname = TimeZoneName{}) const;

    // Return the **Date** and **TimeOfDay** corresponding to this
    // **TimePoint** for the given timezone `tzname`.
    std::pair<Date,TimeOfDay> components(const TimeZoneName& tzname = TimeZoneName{}) const;

    // Return the **TimePoint** corresponding to the most recent midnight for the given
    // timezone `tzname`.
    TimePoint midnight(const TimeZoneName& tzname = TimeZoneName{}) const;

    // Return the **TimePoint** corresponding to the next midnight for the given
    // timezone `tzname`.
    TimePoint next_midnight(const TimeZoneName& tzname = TimeZoneName{}) const;

    // Return the unix timestamp as the real-valued seconds since the
    // epoch.
    double unix_ts() const;

    // Return this **TimePoint** plus the given `duration`.
    template<class Duration>
    requires is_duration_v<Duration>
    TimePoint operator+(Duration duration) const {
	TimePoint tp{*this};
	return tp += duration;
    }

    // Return this **TimePoint** minus the given `duration`.
    template<class Duration>
    requires is_duration_v<Duration>
    TimePoint operator-(Duration duration) const {
	TimePoint tp{*this};
	return tp -= duration;
    }

private:
    TimePointBase *base() { return this; }
};

std::ostream& operator<<(std::ostream& os, const TimePoint& tp);

void to_json(json& j, const TimePoint& tp);
void from_json(const json& j, TimePoint& tp);

}; // core::chrono

namespace core::str::detail {
template<class T> struct lexical_cast_impl;
template<> struct lexical_cast_impl<core::chrono::TimePoint> {
    static core::chrono::TimePoint parse(std::string_view);
};
}; // core::detail

// template <> struct fmt::formatter<chron::TimePoint> : ostream_formatter {};

namespace chron {
using namespace core::chrono;
using namespace core::chrono::literals;
};
