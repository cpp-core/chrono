// Copyright (C) 2019, 2021, 2022 by Mark Melton
//

#include "core/chrono/time_of_day.h"
#include "core/string/lexical_cast.h"

std::strong_ordering operator<=>(const std::chrono::hh_mm_ss<chron::nanos>& a,
				 const std::chrono::hh_mm_ss<chron::nanos>& b) {
    return a.to_duration().count() <=> b.to_duration().count();
}

bool operator==(const std::chrono::hh_mm_ss<chron::nanos>& a,
		const std::chrono::hh_mm_ss<chron::nanos>& b) {
    return a.to_duration().count() == b.to_duration().count();
}

namespace core::chrono
{

TimeOfDay TimeOfDay::min() {
    return TimeOfDay{nanos{0}};
}

TimeOfDay TimeOfDay::max() {
    return TimeOfDay{nanos{24 * 60 * 60 * 1'000'000'000ll - 1}};
}

TimeOfDay::TimeOfDay(const TimeOfDayBase& base)
    : TimeOfDayBase(base) {
}

TimeOfDay::TimeOfDay(int hours, int minutes, int seconds, std::int64_t ns)
    : TimeOfDayBase(nanos{ns + 1'000'000'000ll * (seconds + 60 * (minutes + 60 * hours))}) {
}

std::string_view maybe_consume_to(const char*& ptr, const char *end, char delimeter) {
    const char *begin = ptr;
    while (ptr < end and *ptr != delimeter)
	++ptr;
    std::size_t count = ptr - begin;
    if (ptr != end)
	++ptr;
    return {begin, count};
}

TimeOfDay::TimeOfDay(const std::string& str) {
    const char *ptr = str.data();
    const char *end = ptr + str.size();
    auto hour_str = maybe_consume_to(ptr, end, ':');
    auto min_str = maybe_consume_to(ptr, end, ':');
    auto sec_str = maybe_consume_to(ptr, end, '.');
    auto ns_str = maybe_consume_to(ptr, end, ' ');
    if (ptr != end)
	throw core::runtime_error("TimeOfDay: parse failed: {}", str);
    int hour = hour_str.size() > 0 ? core::str::lexical_cast<int>(hour_str) : 0;
    int min = min_str.size() > 0 ? core::str::lexical_cast<int>(min_str) : 0;
    int sec = sec_str.size() > 0 ? core::str::lexical_cast<int>(sec_str) : 0;
    int ns = ns_str.size() > 0 ? core::str::lexical_cast<int>(ns_str) : 0;
    auto n = ns_str.size();
    while (n++ < 9)
	ns *= 10;
    
    *this = TimeOfDay{hour, min, sec, ns};
}

void to_json(json& j, const TimeOfDay& tod) {
    j = core::str::to_string(tod);
}

void from_json(const json& j, TimeOfDay& tod) {
    tod = TimeOfDay{j.get<std::string>()};
}

std::ostream& operator<<(std::ostream& os, const TimeOfDay& tod) {
    os << fmt::format("{:02d}:{:02d}:{:02d}",
		      tod.hours().count(),
		      tod.minutes().count(),
		      tod.seconds().count());
    if (auto ns = tod.subseconds().count(); ns > 0) {
	int digits = 9;
	while (ns % 10 == 0) {
	    --digits;
	    ns /= 10;
	}
	os << fmt::format(".{:0{}d}", ns, digits);
    }
    return os;
}


}; // core::chrono

namespace core::str::detail {

core::chrono::TimeOfDay lexical_cast_impl<core::chrono::TimeOfDay>::parse(std::string_view s) {
    return core::chrono::TimeOfDay{std::string{s}};
};

}; // core::detail

