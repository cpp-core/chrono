// Copyright (C) 2021, 2022 by Mark Melton
//

#include "core/util/random.h"
#include "core/chrono/time_of_day_stream.h"

namespace coro {

Generator<chron::TimeOfDay> Sampler<chron::TimeOfDay>::operator()
    (chron::TimeOfDay start, chron::TimeOfDay end) const {
    auto sns = start.to_duration().count();
    auto ens = end.to_duration().count();
    auto dist = std::uniform_int_distribution<std::int64_t>(0, ens - sns);
    while (true) {
	auto n = dist(core::rng());
	chron::TimeOfDay tod{chron::nanos{n}};
	co_yield tod;
    }
    co_return;
}

}; // coro
