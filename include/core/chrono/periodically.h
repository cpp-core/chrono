// Copyright (C) 2021, 2022 by Mark Melton
//

#pragma once
#include "core/chrono/chrono.h"

namespace chron
{

class Periodically {
public:
    template<class Duration>
    Periodically(const Duration& period)
	: period_(period)
    { }

    bool operator()() {
	auto tp = chron::TimePoint::now();
	if (tp < next_)
	    return false;
	next_ = tp + period_;
	return true;
    }

private:
    chron::nanos period_;
    chron::TimePoint next_;
};

}; // chron
