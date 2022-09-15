// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include <chrono>
#include <compare>
#include <fmt/format.h>
#include <date/tz.h>
#include "core/util/phantom.h"

namespace core::chrono {

namespace literals {
inline constexpr auto jan = date::January;
inline constexpr auto feb = date::February;
inline constexpr auto mar = date::March;
inline constexpr auto apr = date::April;
inline constexpr auto may = date::May;
inline constexpr auto jun = date::June;
inline constexpr auto jul = date::July;
inline constexpr auto aug = date::August;
inline constexpr auto sep = date::September;
inline constexpr auto oct = date::October;
inline constexpr auto nov = date::November;
inline constexpr auto dec = date::December;
}; // literals

using DateBase = date::year_month_day;
class TimePoint;

// The **TimeZoneName** class is a **Phantom** type (i.e. compile-time only) used to
// differentiate the name of a time zone from a plain std::string. **TimeZoneName** defaults to
// `UTC`.
struct TimeZoneName : public Phantom<std::string> {
    TimeZoneName(const std::string& name = "UTC")
	: Phantom<std::string>(name) {
    }
};

// The **Date** class represents a specific year, month and calendar day. Since **Date**
// does not carry any timezone information, a timezone is required in order to convert
// between **Date** and **TimePoint**.
class Date : public DateBase {
public:
    using DateBase::DateBase;

    // The Date class serves as it's own iterator.
    using value_type = Date;
    using difference_type = size_t;
    using pointer = Date*;
    using reference = Date&;
    using iterator_category = std::forward_iterator_tag;
    value_type operator*() const { return *this; }

    // Return a pointer to the timzone information for the given timezone `tzname`.
    static const date::time_zone *locate_timezone(const std::string& tzname);

    // Return the earliest representable **Date**.
    static Date min();
     
   // Return the latest representable **Date**.
    static Date max();

    // Construct a **Date** from the base class.
    constexpr Date(const DateBase& base)
	: DateBase(base) {
    }

    // Construct a **Date** from a std::chrono::year_month_day.
    Date(const std::chrono::year_month_day& ymd);
    
    // Construct a **Date** from the year, month and day.
    constexpr Date(int year, uint month, uint day)
	: DateBase(date::year{year}, date::month{month}, date::day{day}) {
    }

    // Construct the **Date** corresponding to the **TimePoint** `tp` for the timezone
    // `tzname`.
    explicit Date(const TimePoint& tp, const TimeZoneName& tzname = TimeZoneName{});

    // Construct the **Date** corresponding to `nanos` nanoseconds after the epoch for the
    // timezone `tzname`.
    explicit Date(std::int64_t nanos, const TimeZoneName& tzname = TimeZoneName{});

    // Construct the **Date** corresponding to `ts` seconds after the epoch for the
    // timezone `tzname`.
    explicit Date(double ts, const TimeZoneName& tzname = TimeZoneName{});

    // Construct the current **Date** for the timezone `tzname`.
    Date(const TimeZoneName& tzname = TimeZoneName{});

    // Construct the **Date** corresponding to the std::string `datestr` interpreted using the
    // supplied format `fmt`.
    Date(const std::string& datestr, const std::string& fmt = "%F");

    // Return the **TimePoint** corresponding to midnight for this **Date** in the given
    // timezone `tzname`.
    TimePoint to_timepoint(const TimeZoneName& tzname = TimeZoneName{});

    // Return the unix timestamp as the real-valued seconds since the epoch.
    double unix_ts() const;

    // Return the **Date** for tomorrow.
    Date tomorrow() const;

    // Return the **Date** for yesterday.
    Date yesterday() const;

    // Advance date by one day.
    Date& operator++();

    // Move date backwards by one day.
    Date& operator--();

    // Add `n` days to this **Date**
    Date& operator+=(std::chrono::days n);
    
    // Return a **Date** representing `n` days after this **Date**.
    Date operator+(std::chrono::days n) const;
    
    // Subtract `n` days from this **Date**
    Date& operator-=(std::chrono::days n);

    // Return a **Date** representing `n` days before this **Date**.
    Date operator-(std::chrono::days n) const;

    // Return the duration between **Date** and the `other` **Date**.
    std::chrono::days operator-(Date other) const;

private:
    DateBase *base() { return this; }
};
using Dates = std::vector<Date>;

std::ostream& operator<<(std::ostream& os, const core::chrono::Date& date);

void to_json(json& j, const Date& date);
void from_json(const json& j, Date& date);

}; // core::chrono

namespace core::str::detail {
template<class T> struct lexical_cast_impl;
template<> struct lexical_cast_impl<core::chrono::Date> {
    static core::chrono::Date parse(std::string_view);
};
}; // core::detail

std::ostream& operator<<(std::ostream& os, const std::chrono::year_month_day& ymd);

namespace chron {
using namespace core::chrono;
using namespace core::chrono::literals;
};

template <>
struct fmt::formatter<core::chrono::Date> {
    // Presentation format: 'F' - YYYY-MM-DD.
    char presentation = 'F';

    // Parses format specifications of the form ['F'].
    constexpr auto parse(format_parse_context& ctx) {
	// auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) // c++11
	// [ctx.begin(), ctx.end()) is a character range that contains a part of
	// the format std::string starting from the format specifications to be parsed,
	// e.g. in
	//
	//   fmt::format("{:f} - point of interest", point{1, 2});
	//
	// the range will contain "f} - point of interest". The formatter should
	// parse specifiers until '}' or the end of the range. In this example
	// the formatter should parse the 'f' specifier and return an iterator
	// pointing to '}'.
	
	// Parse the presentation format and store it in the formatter:
	auto it = ctx.begin(), end = ctx.end();
	if (it != end && (*it == 'F' || *it == 'e'))
	    presentation = *it++;
	
	// Check if reached the end of the range:
	if (it != end && *it != '}')
	    throw format_error("invalid format");
	
	// Return an iterator past the end of the parsed range:
	return it;
    }
    
    // Formats the point p using the parsed format specification (presentation)
    // stored in this formatter.
    template <typename FormatContext>
    auto format(const core::chrono::Date& date, FormatContext& ctx) {
	return format_to(ctx.out(),
			 "{:04d}-{:02d}-{:02d}",
			 (int)date.year(), (unsigned int)date.month(), (unsigned int)date.day());
    }
};
