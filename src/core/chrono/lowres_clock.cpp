// Copyright (C) 2017, 2018, 2019, 2021, 2022 by Mark Melton
//

#include "core/chrono/lowres_clock.h"

namespace core::chrono {

LowResClock::LowResClock(Mode mode, chron::nanos resolution)
    : mode_(mode)
    , resolution_(resolution) {
    std::int64_t nanos = chron::TimePoint::now().time_since_epoch().count();
    auto count = resolution.count();
    
    nanos /= count;
    nanos *= count;
    now_.store(chron::TimePoint{nanos}, std::memory_order_release);
    if (mode_ == Mode::RealTime)
	virtual_now_ = now_.load(std::memory_order_acquire);
    
    thread_ = std::thread([this]() {
	while (not done_) {
	    auto next = now_.load(std::memory_order_acquire) + resolution_;
	    std::this_thread::sleep_until(next);
	    now_ = now_.load(std::memory_order_acquire) + resolution_;
	    if (mode_ == Mode::RealTime)
		virtual_now_ = now_.load(std::memory_order_acquire);
	}
    });
}

LowResClock::~LowResClock() {
    done_ = true;
    if (thread_.joinable())
	thread_.join();
}

}; // coros
