// Copyright (C) 2019, 2021, 2022 by Mark Melton
//

#include <date/date.h>
#include <date/tz.h>
#include "core/chrono/date.h"
#include "core/chrono/timepoint.h"
#include "core/util/random.h"
#include "core/util/json.h"
#include "coro/stream/sampler.h"


std::ostream& operator<<(std::ostream& os, const std::chrono::year_month_day& ymd) {
    os << fmt::format("{:04d}-{:02d}-{:02d}", (int)ymd.year(), (uint)ymd.month(), (uint)ymd.day());
    return os;
}

namespace core::chrono
{

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << fmt::format("{:F}", date);
    return os;
}

const date::time_zone *raw_locate_timezone(const std::string& tzname) {
    if (tzname == "EST" or tzname == "EDT") return date::locate_zone("America/New_York");
    if (tzname == "CST" or tzname == "CDT") return date::locate_zone("America/Chicago");
    if (tzname.size() == 0 or tzname == "current") return date::current_zone();
    return date::locate_zone(tzname);
}

const date::time_zone *Date::locate_timezone(const std::string& tzname) {
    auto tz = raw_locate_timezone(tzname);
    if (tz == nullptr)
	throw core::runtime_error("Unrecognzied timezone: {}", tzname);
    return tz;
}

Date Date::min() {
    return Date{date::year::min(), date::month{1}, date::day{1}};
}

Date Date::max() {
    return Date{date::year::max(), date::month{12}, date::day{31}};
}

Date::Date(const TimePoint& tp, const TimeZoneName& tzname) {
    auto zone = locate_timezone(tzname);
    auto zoned_time = date::zoned_time{zone, tp};
    *this = date::floor<std::chrono::days>(zoned_time.get_sys_time());
}

Date::Date(std::int64_t nanos, const TimeZoneName& tzname)
    : Date(TimePoint::epoch() + std::chrono::nanoseconds{nanos}, tzname) {
}

Date::Date(double ts, const TimeZoneName& tzname)
    : Date((std::int64_t)(1e9 * ts), tzname) {
}

Date::Date(const TimeZoneName& tzname)
    : Date(std::chrono::system_clock::now(), tzname) {
}

Date::Date(const std::string& date_str, const std::string& fmt) {
    std::istringstream ss{ date_str };
    ss >> date::parse(fmt, *this);
    if (ss.fail())
	throw core::runtime_error
	    ("error: failed to parse date '{}' using format '{}'", date_str, fmt);
}

TimePoint Date::to_timepoint(const TimeZoneName& tzname) {
    auto tz = locate_timezone(tzname);
    auto zt = date::zoned_time{ tz, (date::local_days)*this };
    return zt.get_sys_time();
}

double Date::unix_ts() const {
    return TimePoint{*this}.unix_ts();
}

Date Date::tomorrow() const {
    return *this + days{1};
}

Date Date::yesterday() const {
    return *this - days{1};
}

Date& Date::operator++() {
    return this->operator+=(std::chrono::days{1});
}

Date& Date::operator--() {
    return this->operator+=(std::chrono::days{-1});
}

Date& Date::operator+=(std::chrono::days duration) {
    *this = date::sys_days(*this) + duration;
    return *this;
}

Date Date::operator+(std::chrono::days duration) const {
    Date date{*this};
    return date += duration;
}

Date& Date::operator-=(std::chrono::days duration) {
    return this->operator+=(-duration);
}

Date Date::operator-(std::chrono::days duration) const {
    Date date{*this};
    return date-=(duration);
}

std::chrono::days Date::operator-(Date other) const {
    return date::sys_days(*this) - date::sys_days(other);
}

void to_json(json& j, const Date& date) {
    j = fmt::format("{}", date);
}

void from_json(const json& j, Date& date) {
    date = Date(j.get<std::string>());
}

}; // core::chrono

namespace core::str::detail {

core::chrono::Date lexical_cast_impl<core::chrono::Date>::parse(std::string_view s) {
    return core::chrono::Date{std::string{s}};
};

}; // core::detail

