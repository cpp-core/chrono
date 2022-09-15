// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include <chrono>
#include <fmt/chrono.h>
#include <ostream>

namespace core::chrono {

template<class T>
struct is_duration : std::false_type {};

template<class R, class P>
struct is_duration<std::chrono::duration<R,P>> : std::true_type {};

template<class T>
constexpr bool is_duration_v = is_duration<T>::value;

using nanos = std::chrono::nanoseconds;
using micros = std::chrono::microseconds;
using millis = std::chrono::milliseconds;
using seconds = std::chrono::seconds;
using minutes = std::chrono::minutes;
using hours = std::chrono::hours;
using days = std::chrono::days;
using months = std::chrono::months;
using years = std::chrono::years;
using DurationAll = std::tuple<nanos, micros, millis, seconds, minutes, hours, days, months, years>;

}; // core::chrono

namespace std::chrono {
template<class Duration>
requires core::chrono::is_duration_v<Duration>
std::ostream& operator<<(std::ostream& os, Duration duration);
}; // std::chrono

namespace chron {
using namespace core::chrono;
};
