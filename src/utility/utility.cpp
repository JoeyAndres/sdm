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

#include "utility/utility.h"

namespace sdm {

bitset<FLOAT_SIZE> FLOATToBitset(FLOAT f) {
  union Converter { std::uint64_t i; FLOAT f; };
  Converter c;
  c.f = f;
  return bitset<FLOAT_SIZE>(c.i);
}

FLOAT bitsetToFLOAT(const bitset<FLOAT_SIZE>& bitset) {
  union Converter { std::uint64_t i; FLOAT f; };
  Converter c;
  c.i = bitset.to_ullong();
  return c.f;
}

}
