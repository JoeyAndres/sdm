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

#include <memory>

#include "./declares.h"
#include "utility/utility.h"
#include "./AddressRegister.h"
#include "./UpDownCounters.h"

using std::shared_ptr;

namespace sdm {

/*!\class SDM
 * \brief The sdm module itself. Aggregates AddressRegister and UpDownCounter.
 * \tparam ADDRESS_BIT_COUNT The bit count of the address data.
 * \tparam HARD_LOCATION_BIT_COUNT Hard location bit count.
 * \tparam DATA_BIT_COUNT Number of bits in the data to be saved.
 *                        Defaults to ADDRESS_BIT_COUNT as in Mr. Karneva's paper.
 */
template <
  size_t ADDRESS_BIT_COUNT,
  size_t HARD_LOCATION_BIT_COUNT,
  size_t DATA_BIT_COUNT = ADDRESS_BIT_COUNT>
class SDM {
 public:
  /**
   * SDM constructor.
   * @param addressRegister AddressRegister to be aggregated.
   * @param upDownCounters UpDownCounters to be aggregated.
   */
  SDM(
    const spAddressRegister<
      ADDRESS_BIT_COUNT, HARD_LOCATION_BIT_COUNT>& addressRegister,
    const spUpDownCounters<
      DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>& upDownCounters,
    size_t threshold);

  /**
   * Writes data to locations selected by address.
   * @param address
   * @param data
   */
  void write(
    const bitset<ADDRESS_BIT_COUNT>& address,
    const bitset<DATA_BIT_COUNT> &data);

  /**
   * Reads data from locations selected by address
   * @param address
   * @return data
   */
  bitset<DATA_BIT_COUNT> read(const bitset<ADDRESS_BIT_COUNT>& address) const;

 protected:
  /**
   * Returns an array of boolean corresponding to each address. True if
   * to update the counter, false otherwise.
   * @param address
   * @return Update flags.
   */
  array<
    bool,
    UpDownCounters<
      DATA_BIT_COUNT,
      HARD_LOCATION_BIT_COUNT>::HARD_LOCATION_COUNT> _getUpdateFlags(
    const bitset<ADDRESS_BIT_COUNT>& address) const;

 protected:
  spAddressRegister<ADDRESS_BIT_COUNT, HARD_LOCATION_BIT_COUNT>
    _addressRegister;
  spUpDownCounters<DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>
    _upDownCounters;
  const size_t _threshold;
};

template <
  size_t ADDRESS_BIT_COUNT,
  size_t HARD_LOCATION_BIT_COUNT,
  size_t DATA_BIT_COUNT = ADDRESS_BIT_COUNT>
using spSDM =
shared_ptr<SDM<ADDRESS_BIT_COUNT, HARD_LOCATION_BIT_COUNT, DATA_BIT_COUNT>>;

template <
  size_t ADDRESS_BIT_COUNT,
  size_t HARD_LOCATION_BIT_COUNT,
  size_t DATA_BIT_COUNT>
SDM<ADDRESS_BIT_COUNT, HARD_LOCATION_BIT_COUNT, DATA_BIT_COUNT>::SDM(
  const spAddressRegister<
    ADDRESS_BIT_COUNT, HARD_LOCATION_BIT_COUNT> &addressRegister,
  const spUpDownCounters<
    DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT> &upDownCounters,
  size_t threshold) :
  _addressRegister(addressRegister),
  _upDownCounters(upDownCounters),
  _threshold(threshold) {
}

template <
  size_t ADDRESS_BIT_COUNT,
  size_t HARD_LOCATION_BIT_COUNT,
  size_t DATA_BIT_COUNT>
void SDM<
  ADDRESS_BIT_COUNT,
  HARD_LOCATION_BIT_COUNT,
  DATA_BIT_COUNT>::write(
  const bitset<ADDRESS_BIT_COUNT> &address,
  const bitset<DATA_BIT_COUNT> &data) {
  auto updateFlags = _getUpdateFlags(address);
  _upDownCounters->write(updateFlags, data);
}

template <
  size_t ADDRESS_BIT_COUNT,
  size_t HARD_LOCATION_BIT_COUNT,
  size_t DATA_BIT_COUNT>
bitset<DATA_BIT_COUNT>
SDM<
  ADDRESS_BIT_COUNT,
  HARD_LOCATION_BIT_COUNT,
  DATA_BIT_COUNT>::read(const bitset<ADDRESS_BIT_COUNT> &address) const {
  auto updateFlags = _getUpdateFlags(address);
  return _upDownCounters->read(updateFlags);
}

template <
  size_t ADDRESS_BIT_COUNT,
  size_t HARD_LOCATION_BIT_COUNT,
  size_t DATA_BIT_COUNT>
array<
  bool,
  UpDownCounters<
    DATA_BIT_COUNT,
    HARD_LOCATION_BIT_COUNT>::HARD_LOCATION_COUNT>
SDM<
  ADDRESS_BIT_COUNT,
  HARD_LOCATION_BIT_COUNT,
  DATA_BIT_COUNT>::_getUpdateFlags(
  const bitset<ADDRESS_BIT_COUNT> &address) const {
  auto hammingDistances =
    _addressRegister->getHammingDistanceArray(address);

  array<
    bool,
    UpDownCounters<
      DATA_BIT_COUNT,
      HARD_LOCATION_BIT_COUNT>::HARD_LOCATION_COUNT> updateFlags;

  for (size_t i = 0; i < hammingDistances.size(); i++) {
    updateFlags[i] = hammingDistances[i] <= _threshold;
  }

  return updateFlags;
}

template <
  size_t ADDRESS_BIT_COUNT,
  size_t HARD_LOCATION_BIT_COUNT,
  size_t DATA_BIT_COUNT>
std::ostream& operator<<(
  std::ostream& os,
  const SDM<
    ADDRESS_BIT_COUNT,
    HARD_LOCATION_BIT_COUNT,
    DATA_BIT_COUNT>& sdm) {
  os << (*sdm._upDownCounters);
  return os;
}

}  // namespace sdm
