// Copyright (C) 2021, 2022 by Mark Melton
//

#include <fmt/format.h>
#include "core/chrono/duration.h"

namespace std::chrono {

std::ostream& output(std::ostream& os, double units, std::string_view name) {
    if (units < 1e1) return os << fmt::format("{:.1f}{}", units, name);
    return os << fmt::format("{:.0f}{}", units, name);
}

template<class Duration>
requires core::chrono::is_duration_v<Duration>
std::ostream& operator<<(std::ostream& os, Duration duration) {
    double units = duration_cast<chron::nanos>(duration).count();
    if (units < 1e3 or units == 0.0) return os << fmt::format("{:.0f}ns", units);
    
    units *= 1e-3;
    if (units < 1e3) return output(os, units, "us");

    units *= 1e-3;
    if (units < 1e3) return output(os, units, "ms");

    units *= 1e-3;
    if (units < 60) return output(os, units, "s");

    return os << fmt::format("{}s", units);
}

template std::ostream& operator<<(std::ostream&, chron::nanos);
template std::ostream& operator<<(std::ostream&, chron::micros);
template std::ostream& operator<<(std::ostream&, chron::millis);
template std::ostream& operator<<(std::ostream&, chron::seconds);
template std::ostream& operator<<(std::ostream&, chron::minutes);
template std::ostream& operator<<(std::ostream&, chron::hours);
template std::ostream& operator<<(std::ostream&, chron::days);
template std::ostream& operator<<(std::ostream&, chron::months);
template std::ostream& operator<<(std::ostream&, chron::years);

}; // std::chrono

