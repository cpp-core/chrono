// Copyright (C) 2021 by Mark Melton
//

#include "core/chrono/date_stream.h"

namespace coro {

Generator<chron::Date> Sampler<core::chrono::Date>::operator()
    (chron::Date start, chron::Date end) const {
    auto sday = std::chrono::sys_days(start).time_since_epoch().count();
    auto eday = std::chrono::sys_days(end).time_since_epoch().count();
    auto dist = std::uniform_int_distribution<int>(0, eday - sday);
    while (true) {
	auto n = dist(core::rng());
	chron::Date date{date::sys_days(start) + std::chrono::days{n}};
	co_yield date;
    }
    co_return;
}

}; // coro
