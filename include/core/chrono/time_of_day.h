// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include <chrono>
#include <compare>
#include "core/chrono/duration.h"
#include "core/util/json.h"

// Compare two **std::chrono::hh_mm_ss**'s.
std::strong_ordering operator<=>(const std::chrono::hh_mm_ss<chron::nanos>& a,
				 const std::chrono::hh_mm_ss<chron::nanos>& b);

// Equality compare two **std::chrono::hh_mm_ss**'s.
bool operator==(const std::chrono::hh_mm_ss<chron::nanos>& a,
		const std::chrono::hh_mm_ss<chron::nanos>& b);

namespace core::chrono {

using TimeOfDayBase = std::chrono::hh_mm_ss<nanos>;

// The TimeOfday class represents a duration from midnight with nanosecond resolution.
class TimeOfDay : public TimeOfDayBase {
public:
    using TimeOfDayBase::TimeOfDayBase;

    // Return the first instant of the day.
    static TimeOfDay min();

    // Return the last instant of the day.
    static TimeOfDay max();

    // Construct a **TimeOfDay** from the base class.
    TimeOfDay(const TimeOfDayBase& base);

    // Construct a **TimeOfDay** from the individual components.
    TimeOfDay(int hours, int minutes, int seconds, std::int64_t ns = 0);

    // Construct a **TimeOfDay** from the supplied `str`.
    TimeOfDay(const std::string& str);
    
    // Compare with other **TimeOfday**.
    auto operator<=>(const TimeOfDay& other) const = default;
    bool operator==(const TimeOfDay& other) const = default;
};

void to_json(json& j, const TimeOfDay& date);
void from_json(const json& j, TimeOfDay& date);

std::ostream& operator<<(std::ostream& os, const TimeOfDay& tod);

}; // core::chrono

namespace core::str::detail {
template<class T> struct lexical_cast_impl;
template<> struct lexical_cast_impl<core::chrono::TimeOfDay> {
    static core::chrono::TimeOfDay parse(std::string_view);
};
}; // core::detail

namespace chron {
using namespace core::chrono;
};
