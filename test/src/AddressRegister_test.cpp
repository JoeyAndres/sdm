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

using std::array;

SCENARIO("Address register converts location addresses to hard location",
         "[sdm::AddressRegister]") {
  GIVEN("Instantiate 256 bit to 2000 location addresses.") {
    constexpr size_t hardLocationBitCount = 11;
    sdm::AddressRegister<256, hardLocationBitCount> addressRegister;
    constexpr size_t hardLocationCount = std::exp2(hardLocationBitCount);

    WHEN("I accessed the location addresses.") {
      array<mpz_class, hardLocationCount> locationAddress;
      mpz_class* lastAddress = &locationAddress.at(0);

      mpz_class offsetAdder;
      mpz_ui_pow_ui(offsetAdder.get_mpz_t(), 2, hardLocationBitCount);

      for (size_t addrIndex = 1;
           addrIndex < locationAddress.size();
           addrIndex++) {
        locationAddress.at(addrIndex) = *lastAddress + offsetAdder;
        lastAddress = &locationAddress.at(addrIndex);
      }
      THEN("Then it contains the appropriate addresses.") {
        REQUIRE(
          locationAddress.size() ==
          addressRegister.getLocationAddresses().size());

        for (size_t addrIndex = 0;
             addrIndex < locationAddress.size();
             addrIndex++) {
          REQUIRE(
            cmp(locationAddress[addrIndex],
                addressRegister.getLocationAddresses()[addrIndex]) == 0);
        }
      }
    }
  }

  GIVEN("Instantiate 8 bit to 4 location addresses.") {
    constexpr size_t hardLocationBitCount = 2;
    sdm::AddressRegister<4, hardLocationBitCount> addressRegister;
    constexpr size_t hardLocationCount = std::exp2(hardLocationBitCount);

    WHEN("I accessed the hamming for 0110, middle case for 8 bit.") {
      THEN("hammingDistanceArray is (2, 2, 3, 2, 3") {
        sdm::hammingDistanceArray<hardLocationCount> hda({2, 1, 3, 2});
        REQUIRE(addressRegister.getHammingDistanceArray(
          std::bitset<hardLocationCount>(std::string("0110"))) ==
          hda);
      }
    }
  }
}
