// Copyright (C) 2019, 2021, 2022 by Mark Melton
//

#pragma once
#include <atomic>
#include <chrono>
#include <fmt/format.h>
#include <iostream>
#include <string>

namespace chron
{

template<class Clock = std::chrono::high_resolution_clock>
class StopWatch
{
public:
    StopWatch()
	: m_start_tp(Clock::now())
	, m_last_tp(m_start_tp)
    { }

    auto now() const
    {
	std::atomic_thread_fence(std::memory_order_relaxed);
	auto current_tp = Clock::now();
	std::atomic_thread_fence(std::memory_order_relaxed);
	return current_tp;
    }

    auto mark()
    {
	auto current_tp = now();
	auto elapsed = current_tp - m_last_tp;
	m_last_tp = current_tp;
	return elapsed;
    }

    template<class Units = typename Clock::duration>
    auto elapsed_time()
    {
	auto elapsed = mark();
	return std::chrono::duration_cast<Units>(elapsed).count();
    }
    
    template<class Units = typename Clock::duration>
    auto elapsed_time(std::ostream& os, std::string_view msg = "{}")
    {
	auto duration = elapsed_time<Units>();
	os << fmt::format(msg, duration) << std::endl;
	mark();
    }

    // Return the total number of nanos since the StopWatch was
    // constructed.
    template<class Units = typename Clock::duration>
    auto total_elapsed_time()
    {
	return std::chrono::duration_cast<Units>(now() - m_start_tp).count();
    }
    
private:
    typename Clock::time_point m_start_tp;
    typename Clock::time_point m_last_tp;
};

}; // chron

