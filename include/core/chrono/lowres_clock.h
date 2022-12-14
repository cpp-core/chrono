// Copyright (C) 2017, 2018, 2019, 2021, 2022 by Mark Melton
//

#pragma once
#include <thread>
#include "core/chrono/chrono.h"

namespace core::chrono {

// LowResClock provides very fast access to the current time with
// configurable resolution (typically 1ms). This provides an
// alternative to the standard system time which has high resolution
// but requires upwards of tens of nanos to access.
//
// The clock has two modes of operation: RealTime and Virtual. In
// RealTime mode, both rnow() and vnow() return the current time. In
// Virtual mode, rnow() still returns the real time, but vnow()
// returns the manually set virtual time.
//
// The clocks tracks real time modulo the given resolution using a
// separate thread that is periodically awakened and updates the
// internal time point using the high resolution system clock.
class LowResClock {
public:
    enum class Mode { RealTime, Virtual };

    // Construct a clock with the given <mode> and <resolution>.
    LowResClock(Mode mode, chron::nanos resolution);
    ~LowResClock();

    // Disable copy and move because of the internal thread that
    // depends on the original object remaining valid.
    LowResClock(const LowResClock&) = delete;
    LowResClock& operator=(const LowResClock&) = delete;

    // Return the clock mode: RealTime or Virtual.
    auto mode() const { return mode_; }

    // Return the clock resolution.
    chron::nanos resolution() const { return resolution_; }

    // Return the current virtual time.
    chron::TimePoint virtual_now() const { return virtual_now_; }
    
    // Return the current actual time.
    chron::TimePoint now() const { return now_; }

    // Set the current virtual time. Must be in Virtual mode.
    void virtual_now(chron::TimePoint tp) {
	assert(mode_ == Mode::Virtual);
	virtual_now_ = tp;
    }
    
private:
    Mode mode_;
    chron::nanos resolution_;
    std::atomic<chron::TimePoint> now_, virtual_now_;
    std::thread thread_;
    std::atomic<bool> done_{false};
};

}; // cot
