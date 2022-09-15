// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "core/chrono/time_of_day.h"
#include "coro/stream/sampler.h"

namespace coro {

template<>
struct Sampler<chron::TimeOfDay> {
    Generator<chron::TimeOfDay> operator()
    (chron::TimeOfDay start = chron::TimeOfDay::min(),
     chron::TimeOfDay end = chron::TimeOfDay::max()) const;
};

}; // coro
