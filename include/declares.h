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

using std::array;

namespace sdm {

constexpr size_t BYTE_BIT_SIZE = 8;

using FLOAT = double;
constexpr size_t FLOAT_SIZE = sizeof(FLOAT) * BYTE_BIT_SIZE;

/*! \typedef floatArray
 *  \brief An array of float.
 *  \tparam N Number of element.
 */
template <size_t N>
using floatArray = std::array<FLOAT, N>;

template<size_t N>
using hammingDistanceArray = array<size_t, N>;

}  // namespace sdm
