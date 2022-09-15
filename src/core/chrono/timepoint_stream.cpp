// Copyright (C) 2021, 2022 by Mark Melton
//

#include "core/chrono/timepoint_stream.h"

namespace coro {

Generator<chron::TimePoint> Sampler<chron::TimePoint>::operator()
    (chron::TimePoint start, chron::TimePoint end) const {
    auto sns = start.time_since_epoch().count();
    auto ens = end.time_since_epoch().count();
    auto dist = std::uniform_int_distribution<std::int64_t>(0, ens - sns);
    while (true) {
	auto n = dist(core::rng());
	core::chrono::TimePoint tp{n};
	co_yield tp;
    }
    co_return;
}

}; // coro
