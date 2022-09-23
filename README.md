[![Build and Unit Test](https://github.com/cpp-core/chrono/actions/workflows/build.yaml/badge.svg)](https://github.com/cpp-core/chrono/actions/workflows/build.yaml)

# C++ Timepoint and Date Library

## At A Glance

## Installation

    git clone https://github.com/cpp-core/depends
	mkdir depends/build && cd depends/build
    CC=clang-mp-11 CXX=clang++-mp-11 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt -DDATE_TEST=ON ..
	make date-check # Run tests
	make date       # Build and install
