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

#include <array>
#include <cmath>
#include <bitset>
#include <memory>
#include <iostream>

#include "./declares.h"
#include "utility/utility.h"

using std::array;
using std::bitset;
using std::shared_ptr;

namespace sdm {

/*!\class UpDownCounters
 * \brief Updown counters for sdm.
 * \tparam DATA_BIT_COUNT Bit count of the data to be saved/retrieved.
 * \tparam HARD_LOCATION_BIT_COUNT Bit count of the hard location.
 */
template <
  size_t DATA_BIT_COUNT,
  size_t HARD_LOCATION_BIT_COUNT>
class UpDownCounters {
 public:
  static constexpr size_t HARD_LOCATION_COUNT =
    std::exp2(HARD_LOCATION_BIT_COUNT);

  /**
   * No-arg constructor.
   */
  explicit UpDownCounters(FLOAT geometricRatio);

  /**
   * Input the bits given an array of hamming distance.
   * @param updateFlags Array of boolean indicating whether to update.
   * @param bits Input bits.
   */
  void write(const array<bool, HARD_LOCATION_COUNT>& updateFlags,
             const bitset<DATA_BIT_COUNT>& bits);

  /**
   * Output the bits given an array of hamming distance.
   * @param updateFlags Array of boolean indicating whether to update.
   * @return The output.
   */
  bitset<DATA_BIT_COUNT> read(
    const array<bool, HARD_LOCATION_COUNT>& updateFlags) const;

  /**
   * @return Counter grid.
   */
  const array<
    array<FLOAT, DATA_BIT_COUNT>, HARD_LOCATION_COUNT>& getCounters() const;

 protected:
  void _writeRow(const bitset<DATA_BIT_COUNT>& bits, size_t row);

  array<FLOAT, DATA_BIT_COUNT> _readRow(size_t row) const;

 protected:
  FLOAT _geometricRatio;
  array<array<FLOAT, DATA_BIT_COUNT>, HARD_LOCATION_COUNT> _upDownCounters;
};

/*!\typedef spUpDownCounters
 * \brief Wraps UpDownCounters with shared_ptr.
 * \tparam DATA_BIT_COUNT Number of bit in data to be saved.
 * \tparam HARD_LOCATION_BIT_COUNT Number of hard location bit.
 */
template <
  size_t DATA_BIT_COUNT,
  size_t HARD_LOCATION_BIT_COUNT>
using spUpDownCounters =
shared_ptr<UpDownCounters<DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>>;

template <size_t DATA_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
UpDownCounters<DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>::UpDownCounters(
  FLOAT geometricRatio) :
  _geometricRatio(geometricRatio) {
  for (array<FLOAT, DATA_BIT_COUNT>& udc : _upDownCounters) {
    udc.fill(0);
  }
}

template <size_t DATA_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
void UpDownCounters<DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>::write(
  const array<
    bool,
    UpDownCounters<
      DATA_BIT_COUNT,
      HARD_LOCATION_BIT_COUNT>::HARD_LOCATION_COUNT>& updateFlags,
  const bitset<DATA_BIT_COUNT> &bits) {
  for (size_t i = 0; i < updateFlags.size(); i++) {
    if (updateFlags[i]) {
      this->_writeRow(bits, i);
    }
  }
}

template <size_t DATA_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
bitset<DATA_BIT_COUNT>
UpDownCounters<DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>::read(
  const array<
    bool,
    UpDownCounters<
      DATA_BIT_COUNT,
      HARD_LOCATION_BIT_COUNT>::HARD_LOCATION_COUNT>& updateFlags) const {
  array<FLOAT, DATA_BIT_COUNT> sumArray;
  sumArray.fill(0);
  for (size_t i = 0; i < updateFlags.size(); i++) {
    if (updateFlags[i]) {
      sumArray = this->_readRow(i) + sumArray;
    }
  }

  bitset<DATA_BIT_COUNT> bits;
  for (size_t i = 0; i < sumArray.size(); i++) {
    bits[i] = sumArray[i] > 0 ? 1 : 0;
  }

  return bits;
}

template <size_t DATA_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
void UpDownCounters<DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>::_writeRow(
  const bitset<DATA_BIT_COUNT>& bits,
  size_t row) {
  array<FLOAT, DATA_BIT_COUNT>& rowUpDownCounters = _upDownCounters.at(row);
  for (size_t i = 0; i < bits.size(); i++) {
    FLOAT direction = bits[i] ? 1.0F : -1.0F;
    rowUpDownCounters.at(i) =
      rowUpDownCounters.at(i) +
        _geometricRatio * (direction - rowUpDownCounters.at(i));
  }
}

template <size_t DATA_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
array<FLOAT, DATA_BIT_COUNT>
UpDownCounters<DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>::_readRow(
  size_t row) const {
  array<FLOAT, DATA_BIT_COUNT> sumArray;
  sumArray.fill(0);
  auto rowUpDownCounters = _upDownCounters.at(row);
  for (size_t i = 0; i < rowUpDownCounters.size(); i++) {
    sumArray.at(i) = rowUpDownCounters.at(i);
  }

  return sumArray;
}

template <size_t DATA_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
const array<
  array<FLOAT, DATA_BIT_COUNT>,
  UpDownCounters<
    DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>::HARD_LOCATION_COUNT>&
UpDownCounters<DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>::getCounters() const {
  return _upDownCounters;
}

template <size_t DATA_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
std::ostream& operator<<(
  std::ostream& os,
  const UpDownCounters<
    DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>& upDownCounters) {
  for (auto row : upDownCounters.getCounters()) {
    for (auto col : row) {
      os << col;
    }
    os << std::endl;
  }
  return os;
}

}  // namespace sdm
