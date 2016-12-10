/**
 * sdm - Sparse Distributed Memory
 * Copyright (C) 2016  Joey Andres<yeojserdna@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gmpxx.h>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>

#include "sdm"

#include "catch.hpp"

using std::array;

union Converter { std::uint64_t i; double d; };

SCENARIO("UpDownCounters data storage.",
         "[sdm::UpDownCounters]") {
  GIVEN("Instantiate 64 (double) bit to 2000 location addresses.") {
    constexpr size_t hardLocationBitCount = 3;
    sdm::UpDownCounters<64, hardLocationBitCount> upDownCounters(0.1F);

    WHEN("Base case: When no insertion done.") {
      THEN("I get 0") {
        auto acquiredData = upDownCounters.getData({0, 1, 0, 0, 1, 0, 1, 0});
        Converter c2;
        c2.i =  acquiredData.to_ullong();
        REQUIRE(c2.d == sdm::FLOAT(0.0F));
      }
    }

    WHEN("Base case: When I set a memory to 1.") {
      Converter c1;
      c1.d = sdm::FLOAT(1.0F);
      upDownCounters.insertData({0, 1, 0, 0, 1, 0, 1, 0}, c1.i);
      THEN("I get 1") {
        auto acquiredData = upDownCounters.getData({0, 1, 0, 0, 1, 0, 1, 0});
        Converter c2;
        c2.i =  acquiredData.to_ullong();
        REQUIRE(c2.d == sdm::FLOAT(1.0F));
      }
    }

    WHEN("I reinforced memory with -1 more often than 1.") {
      constexpr size_t hardLocationBitCount = 3;
      sdm::UpDownCounters<64, hardLocationBitCount> upDownCounters(0.1F);

      Converter c1;
      c1.d = sdm::FLOAT(-1.0F);
      upDownCounters.insertData({0, 1, 0, 0, 1, 0, 1, 0}, c1.i);
      upDownCounters.insertData({0, 1, 0, 0, 1, 0, 1, 0}, c1.i);

      c1.d = sdm::FLOAT(1.0F);
      upDownCounters.insertData({0, 1, 0, 0, 1, 0, 1, 0}, c1.i);

      THEN("I get -1") {
        auto acquiredData = upDownCounters.getData({0, 1, 0, 0, 1, 0, 1, 0});
        Converter c2;
        c2.i =  acquiredData.to_ullong();
        REQUIRE(c2.d == sdm::FLOAT(-1.0F));
      }
    }
  }
}
