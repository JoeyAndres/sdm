#!/usr/bin/env bash

cd /home/tester/sdm && mkdir build && cd build && cmake .. && make && ./test/testRunner
