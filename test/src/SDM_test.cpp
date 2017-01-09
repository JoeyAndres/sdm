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

#include "sdm"

#include "catch.hpp"

union Converter { std::uint64_t i; double d; };

SCENARIO("SDM read/write",
         "[sdm::SDM]") {
  GIVEN("Instantiate 4 bit address to hard 3 bit location addresses") {
    constexpr size_t hardLocationBitCount = 3;
    constexpr size_t addressBitCount = 4;
    constexpr size_t dataBitCount = 4;
    auto sparseDistributedSystem = sdm::SDMFactory<
      addressBitCount,
      hardLocationBitCount,
      dataBitCount>(1).get();

    WHEN("Base case: When no insertion done.") {
      auto acquiredData =
        sparseDistributedSystem->read(bitset<4>("1000"));
      THEN("I get 0") {
        REQUIRE(acquiredData == bitset<4>("0000"));
      }
    }

    WHEN("Base case: When I set a memory to 1") {
      auto addr1 = bitset<4>("1000");
      auto data1 = bitset<4>("1001");
      sparseDistributedSystem->write(addr1, data1);

      REQUIRE(sparseDistributedSystem->read(addr1) == data1);

      // Sanity check.
      auto addr2 = ~addr1;
      REQUIRE(sparseDistributedSystem->read(addr2) == bitset<4>("0000"));
    }
  }

  GIVEN("Instantiate 64 bit address to hard 2000 location addresses and 64bit "
          "data") {
    constexpr size_t hardLocationBitCount = 10;
    constexpr size_t addressBitCount = 64;
    constexpr size_t dataBitCount = 64;
    auto sparseDistributedSystem = sdm::SDMFactory<
      addressBitCount,
      hardLocationBitCount,
      dataBitCount>(32).get();

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

    WHEN("Base case: When I set a memory to 2.") {
      THEN("I get 1") {
        Converter c1;
        c1.d = 2.0F;
        sparseDistributedSystem->write(c1.i, c1.i);
        auto acquiredData =
          sparseDistributedSystem->read(c1.i);
        Converter c2;
        c2.i =  acquiredData.to_ullong();
        REQUIRE(c2.d == sdm::FLOAT(2.0F));

        // Sanity check.
        bitset<64> bitset1 = c1.i;
        bitset<64> flipBit = ~bitset1;
        sparseDistributedSystem->write(flipBit, sdm::FLOATToBitset(0.0F));
        auto acquiredFlippedData = sparseDistributedSystem->read(flipBit);
        c2.i = acquiredFlippedData.to_ullong();
        REQUIRE(c2.d == sdm::FLOAT(0.0F));
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

  GIVEN("Instantiate 192 bit (3 float) address to hard 2000 location addresses "
          "and 64bit data") {
    constexpr size_t hardLocationBitCount = 18;
    constexpr size_t addressBitCount = 192;
    constexpr size_t dataBitCount = 64;
    auto sparseDistributedSystem = sdm::SDMFactory<
      addressBitCount,
      hardLocationBitCount,
      dataBitCount>(68).get();

    sdm::floatArray<3> floatArray1 { 0.1F, 0.2F, 0.3F };
    sdm::floatArray<3> floatArray2 { 10.2F, -3000.2F, 100.0F };

    auto addr1 = sdm::floatArrayToBitset(floatArray1);
    auto addr2 = sdm::floatArrayToBitset(floatArray2);

    WHEN("Nothing is written") {
      THEN("I should retrieve zero.") {
        REQUIRE(sdm::bitsetToFLOAT(sparseDistributedSystem->read(addr1)) ==
          0.0F);
        REQUIRE(sdm::bitsetToFLOAT(sparseDistributedSystem->read(addr2)) ==
          0.0F);
      }
    }

    WHEN("I associate floatArray1 and value 2.0F") {
      sparseDistributedSystem->write(addr1, sdm::FLOATToBitset(2.0F));
      sparseDistributedSystem->write(addr2, sdm::FLOATToBitset(0.0F));
      THEN("I should be able to retrieve 2.0 back.") {
        REQUIRE(sdm::bitsetToFLOAT(sparseDistributedSystem->read(addr1)) ==
          2.0F);
        REQUIRE(sdm::bitsetToFLOAT(sparseDistributedSystem->read(addr2)) ==
          0.0F);
      }
    }

    WHEN("I associate floatArray2 and value -69") {
      sparseDistributedSystem->write(addr1, sdm::FLOATToBitset(2.0F));
      sparseDistributedSystem->write(addr2, sdm::FLOATToBitset(-69.0F));
      THEN("I should be able to retrieve -69 back.") {
        REQUIRE(sdm::bitsetToFLOAT(sparseDistributedSystem->read(addr1)) ==
          2.0F);
        REQUIRE(sdm::bitsetToFLOAT(sparseDistributedSystem->read(addr2)) ==
          -69.0F);
      }
    }
  }
}
