// Copyright (C) 2021, 2022 by Mark Melton
//

#include "core/chrono/timepoint.h"
#include "core/string/lexical_cast.h"

namespace core::chrono
{

std::ostream& operator<<(std::ostream& os, const TimePoint& tp) {
    os << tp.to_string();
    return os;
}

TimePoint TimePoint::epoch() {
    return TimePoint{std::chrono::nanoseconds{0}};
}
    
TimePoint TimePoint::now() {
    return TimePoint{std::chrono::system_clock::now()};
}
    
TimePoint TimePoint::max() {
    return TimePoint{std::chrono::nanoseconds{std::numeric_limits<std::int64_t>::max()}};
}

TimePoint::TimePoint(const TimePointBase& base)
    : TimePointBase(base) {
}

TimePoint::TimePoint(std::int64_t nanos)
    : TimePointBase(std::chrono::nanoseconds{nanos}) {
}

TimePoint::TimePoint(double ts)
    : TimePointBase(std::chrono::nanoseconds{std::int64_t(1e9 * ts)}) {
}

TimePoint::TimePoint(const Date& date, const TimeZoneName& tzname) {
    auto tz = Date::locate_timezone(tzname);
    auto zt = date::zoned_time{tz, (date::local_days)date};
    *this = zt.get_sys_time();
}

TimePoint::TimePoint(const Date& date, const TimeOfDay& tod, const TimeZoneName& tzname)
    : TimePoint(date, tzname) {
    *this += tod.to_duration();
}

TimePoint::TimePoint(const std::string& str, const TimeZoneName& tzname, const std::string& fmt) {
    auto auto_fmt = fmt.size() == 0 ? (str.size() < 14 ? "%F" : "%F %T") : fmt;
    std::string tzn;
    std::istringstream ss{str};
    date::local_time<std::chrono::nanoseconds> tp;
    ss >> date::parse(auto_fmt, tp, tzn);

    if (ss.fail()) 
	throw runtime_error("failed to parse timepoint: {} with format {}", str, fmt);

    if (tzn.size() == 0)
	tzn = tzname;
    auto tz = Date::locate_timezone(tzn);
    auto zt = date::zoned_time{tz, tp};
    *this = zt.get_sys_time();
}

std::string TimePoint::to_string(const TimeZoneName& tzname, const std::string& fmt) const {
    auto tz = Date::locate_timezone(tzname);
    auto zt = date::zoned_time{tz, *this};
    return date::format(fmt, zt);
}

Date TimePoint::date(const TimeZoneName& tzname) const {
    auto tz = Date::locate_timezone(tzname);
    auto zt = date::zoned_time{tz, *this};
    return Date{date::floor<date::days>(zt.get_local_time())};
}

TimeOfDay TimePoint::time_of_day(const TimeZoneName& tzname) const {
    auto interval = *this - midnight(tzname);
    return TimeOfDay{interval};
}

std::pair<Date,TimeOfDay> TimePoint::components(const TimeZoneName& tzname) const {
    Date d = date(tzname);
    auto tz = Date::locate_timezone(tzname);
    auto zt = date::zoned_time{tz, (date::local_days)d};
    auto interval = *this - zt.get_sys_time();
    TimeOfDay tod{interval};
    return {d, tod};
}

TimePoint TimePoint::midnight(const TimeZoneName& tzname) const {
    return TimePoint{date(tzname), tzname};
}

TimePoint TimePoint::next_midnight(const TimeZoneName& tzname) const {
    auto d = date(tzname);
    ++d;
    return TimePoint{d, tzname};
}

double TimePoint::unix_ts() const {
    return 1e-9 * time_since_epoch().count();
}

void to_json(json& j, const TimePoint& tp) {
    j = core::str::to_string(tp);
}

void from_json(const json& j, TimePoint& tp) {
    tp = TimePoint{j.get<std::string>()};
}

}; // core::chrono

namespace core::str::detail {

core::chrono::TimePoint lexical_cast_impl<core::chrono::TimePoint>::parse(std::string_view s) {
    return core::chrono::TimePoint{std::string{s}};
};

}; // core::detail




