// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include "core/util/random.h"
#include "core/chrono/date.h"
#include "core/chrono/timepoint.h"
#include "coro/stream/sampler.h"

namespace coro {

template<>
struct Sampler<chron::TimePoint> {
    Generator<chron::TimePoint> operator()
    (chron::TimePoint start = chron::TimePoint{chron::jan/1/1990},
     chron::TimePoint end = chron::TimePoint{chron::jan/1/2040}) const;
};

}; // coro
