// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include "core/util/random.h"
#include "core/chrono/duration.h"
#include "coro/stream/sampler.h"

namespace coro {

template<class T>
requires chron::is_duration_v<T>
struct Sampler<T> {
    Generator<T> operator()(T start = T::min(), T end = T::max()) const {
	auto sn = start.count();
	auto en = end.count();
	auto dist = std::uniform_int_distribution<int>(0, en - sn);
	while (true) {
	    auto n = sn + dist(core::rng());
	    T duration{n};
	    co_yield duration;
	}
	co_return;
    };
};

}; // coro
