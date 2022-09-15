// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include "core/util/random.h"
#include "core/chrono/date.h"
#include "coro/stream/sampler.h"

namespace coro {

template<>
struct Sampler<core::chrono::Date> {
    Generator<chron::Date> operator()
    (chron::Date start = chron::jan/1/1990,
     chron::Date end = chron::jan/1/2040) const;
};

}; // coro
