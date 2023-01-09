[![Build and Unit Test](https://github.com/cpp-core/chrono/actions/workflows/build.yaml/badge.svg)](https://github.com/cpp-core/chrono/actions/workflows/build.yaml)

# C++ Timepoint and Date Library

## At A Glance

## Installation

	git clone git@github.com:cpp-core/chrono
	mkdir chrono/build && cd chrono/build
	CC=clang-mp-14 CXX=clang++-mp-14 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt ..
	make check   # Run tests
	make install # Build and install
