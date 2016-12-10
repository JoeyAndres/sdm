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

#pragma once

#include <gmpxx.h>

#include <bitset>
#include <array>
#include <cmath>
#include <iostream>
#include <memory>

#include "./declares.h"

using std::bitset;
using std::array;
using std::shared_ptr;

namespace sdm {

/*!\class AddressRegister
 * \brief Represents the address register for sdm.
 * \tparam ADDRESS_BIT_COUNT The bit count of the address data.
 * \tparam HARD_LOCATION_BIT_COUNT Hard location bit count.
 */
template<size_t ADDRESS_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
class AddressRegister {
 public:
  static constexpr size_t HARD_LOCATION_COUNT =
    std::exp2(HARD_LOCATION_BIT_COUNT);

  /**
   * No-arg constructor.
   */
  AddressRegister();

  /**
   * Acquires the hammingDistanceArray given an address.
   * @param bits The address data.
   * @return hammingDistanceArray.
   */
  hammingDistanceArray<HARD_LOCATION_COUNT> getHammingDistanceArray(
      const bitset<ADDRESS_BIT_COUNT>& bits) const;
  hammingDistanceArray<HARD_LOCATION_COUNT> getHammingDistanceArray(
    const mpz_class& bits) const;

  const array<mpz_class, HARD_LOCATION_COUNT>& getLocationAddresses() const;
  array<mpz_class, HARD_LOCATION_COUNT>& getLocationAddresses();

 protected:
  array<mpz_class, HARD_LOCATION_COUNT> _locationAddresses;
};

/*!\typedef spAddressRegister
 * \brief Wraps AddressRegister in shared_ptr.
 * \tparam ADDRESS_BIT_COUNT The bit count of the address data.
 * \tparam HARD_LOCATION_BIT_COUNT Hard location bit count.
 */
template<size_t ADDRESS_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
using spAddressRegister =
shared_ptr<AddressRegister<ADDRESS_BIT_COUNT, HARD_LOCATION_BIT_COUNT>>;

template<size_t ADDRESS_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
AddressRegister<ADDRESS_BIT_COUNT, HARD_LOCATION_BIT_COUNT>::AddressRegister() {
  // Initialize all _locationAddresses to 0.
  for (mpz_class& hardAddress : _locationAddresses) {
    hardAddress = 0;
  }

  mpz_class* lastAddress = &_locationAddresses.at(0);

  mpz_class offsetAdder;
  mpz_ui_pow_ui(offsetAdder.get_mpz_t(), 2, HARD_LOCATION_BIT_COUNT);

  for (size_t addrIndex = 1;
       addrIndex < _locationAddresses.size();
       addrIndex++) {
    _locationAddresses.at(addrIndex) = *lastAddress + offsetAdder;
    lastAddress = &_locationAddresses.at(addrIndex);
  }
}

template<size_t ADDRESS_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
hammingDistanceArray<
  AddressRegister<ADDRESS_BIT_COUNT,
                  HARD_LOCATION_BIT_COUNT>::HARD_LOCATION_COUNT>
AddressRegister<ADDRESS_BIT_COUNT,
                HARD_LOCATION_BIT_COUNT>::getHammingDistanceArray(
  const bitset<ADDRESS_BIT_COUNT>& bits) const {
  auto bitStr = bits.to_string();
  mpz_class mpBit(bitStr, 2);

  hammingDistanceArray<AddressRegister<
    ADDRESS_BIT_COUNT,
    HARD_LOCATION_BIT_COUNT>::HARD_LOCATION_COUNT> hda;
  for (size_t addrIndex = 0;
       addrIndex < _locationAddresses.size();
       addrIndex++) {
    hda[addrIndex] = mpz_hamdist(mpBit.get_mpz_t(),
                                 _locationAddresses[addrIndex].get_mpz_t());
  }

  return hda;
}
template<size_t ADDRESS_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
hammingDistanceArray<
  AddressRegister<ADDRESS_BIT_COUNT,
                  HARD_LOCATION_BIT_COUNT>::HARD_LOCATION_COUNT>
  AddressRegister<ADDRESS_BIT_COUNT,
                  HARD_LOCATION_BIT_COUNT>::getHammingDistanceArray(
  const mpz_class& bits) const {
  hammingDistanceArray<AddressRegister<
    ADDRESS_BIT_COUNT,
    HARD_LOCATION_BIT_COUNT>::HARD_LOCATION_COUNT> hda;
  for (size_t addrIndex = 0;
       addrIndex < _locationAddresses.size();
       addrIndex++) {
    hda[addrIndex] = mpz_hamdist(bits.get_mpz_t(),
                                 _locationAddresses[addrIndex].get_mpz_t());
  }

  return hda;
}

template<size_t ADDRESS_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
const array<
  mpz_class,
  AddressRegister<
    ADDRESS_BIT_COUNT,
    HARD_LOCATION_BIT_COUNT>::HARD_LOCATION_COUNT>&
AddressRegister<ADDRESS_BIT_COUNT,
                HARD_LOCATION_BIT_COUNT>::getLocationAddresses() const {
  return this->_locationAddresses;
}

template<size_t ADDRESS_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
array<
  mpz_class,
  AddressRegister<
    ADDRESS_BIT_COUNT,
    HARD_LOCATION_BIT_COUNT>::HARD_LOCATION_COUNT>&
AddressRegister<ADDRESS_BIT_COUNT,
                HARD_LOCATION_BIT_COUNT>::getLocationAddresses() {
  return this->_locationAddresses;
}

}  // namespace sdm
