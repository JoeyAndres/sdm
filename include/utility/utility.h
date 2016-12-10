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

using std::array;

namespace sdm {

template<typename T, size_t N>
array<T, N> operator+(const array<T, N>& lhs, const array<T, N>& rhs) {
  array<T, N> rv;
  for (size_t i = 0; i < N; i++) {
    rv[i] = lhs[i] + rhs[i];
  }

  return rv;
}

template<typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const array<T, N>& op) {
  for (size_t i = 0; i < N; i++) {
    os << op[i] << " ";
  }

  return os;
}

}  // namespace sdm
