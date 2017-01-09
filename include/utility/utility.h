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
#include <iostream>
#include <bitset>

#include "../declares.h"

using std::array;
using std::bitset;

namespace sdm {

/**
 * Adds each corresponding elements of array of the same size.
 * @tparam T Data type of elements in array.
 * @tparam N Size of array.
 * @param lhs LHS array.
 * @param rhs RHS array.
 * @return Array in which corresponding elements are added.
 */
template<typename T, size_t N>
array<T, N> operator+(const array<T, N>& lhs, const array<T, N>& rhs) {
  array<T, N> rv;
  for (size_t i = 0; i < N; i++) {
    rv[i] = lhs[i] + rhs[i];
  }

  return rv;
}

/**
 * Stream for array. For debugging purposes.
 * @tparam T Data type of elements in array.
 * @tparam N Size of array.
 * \param os stream.
 * \param op array.
 * @return stream.
 */
template<typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const array<T, N>& op) {
  for (size_t i = 0; i < N; i++) {
    os << op[i] << " ";
  }

  return os;
}

/**
 * Converts a float to bitset.
 * @param f Floating point.
 * @return bitset equivalent of the given f.
 */
bitset<FLOAT_SIZE> FLOATToBitset(FLOAT f);

/**
 * Converts a bitset of size FLOAT_SIZE to FLOAT.
 * @param bitset The bitset version of float.
 * @return Float equivalent of the bitset.
 */
FLOAT bitsetToFLOAT(const bitset<FLOAT_SIZE>& bitset);

/**
 * Converts a floatArray to a bitset.
 * @tparam D Number of elements in the floatArray.
 * @param fv FloatArray.
 * @return Converted fv to bitset.
 */
template <size_t D>
bitset<D * FLOAT_SIZE> floatArrayToBitset(const floatArray<D>& fv) {
  bitset<D * FLOAT_SIZE> rv;
  union Converter { FLOAT f; uint64_t i; };
  for (size_t d = 0; d < D; d++) {
    size_t dRev = (D - 1) - d;
    Converter c;
    c.f = fv[d];
    bitset<FLOAT_SIZE> tempBitset = c.i;

    for (size_t b = 0; b < tempBitset.size(); b++) {
      rv[FLOAT_SIZE * dRev + b] = tempBitset[b];
    }
  }
  return rv;
}

}  // namespace sdm
