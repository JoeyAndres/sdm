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

#include "sdm"

#include "catch.hpp"

union Converter { std::uint64_t i; double d; };

SCENARIO("SDM read/write",
         "[sdm::SDM]") {
  GIVEN("Instantiate 256 bit address to hard 2000 location addresses and 64bit "
          "data") {
    constexpr size_t hardLocationBitCount = 11;
    constexpr size_t addressBitCount = 64;
    constexpr size_t dataBitCount = 64;
    auto sparseDistributedSystem = sdm::SDMFactory<
      addressBitCount,
      hardLocationBitCount,
      dataBitCount>(3).get();

    WHEN("Base case: When no insertion done.") {
      THEN("I get 0") {
        Converter c1;
        c1.d = 1.0F;
        auto acquiredData =
          sparseDistributedSystem->read(c1.i);
        Converter c2;
        c2.i =  acquiredData.to_ullong();
        REQUIRE(c2.d == sdm::FLOAT(0.0F));
      }
    }

    WHEN("Base case: When I set a memory to 1.") {
      THEN("I get 1") {
        Converter c1;
        c1.d = 1.0F;
        sparseDistributedSystem->write(c1.i, c1.i);
        auto acquiredData =
          sparseDistributedSystem->read(c1.i);
        Converter c2;
        c2.i =  acquiredData.to_ullong();
        REQUIRE(c2.d == sdm::FLOAT(1.0F));
      }
    }

    WHEN("I reinforced memory with -1 more often than 1.") {
      Converter c1;
      c1.d = 1.0F;
      Converter c2;
      c2.d = -1.0F;

      sparseDistributedSystem->write(c1.i, c1.i);
      sparseDistributedSystem->write(c1.i, c1.i);

      sparseDistributedSystem->write(c1.i, c2.i);
      sparseDistributedSystem->write(c1.i, c2.i);

      auto acquiredData =
        sparseDistributedSystem->read(c1.i);

      Converter c3;
      c3.i = acquiredData.to_ullong();

      THEN("Then I get -1") {
        REQUIRE(c3.d == sdm::FLOAT(-1.0F));
      }
    }
  }
}
