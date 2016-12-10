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

#include "./declares.h"
#include "./utility/FactoryAbstract.h"
#include "UpDownCounters.h"

namespace sdm {

/*!\class UpDownCountersFactory
 * \brief Factory method for UpDownCounters
 * \tparam DATA_BIT_COUNT Bit count of the data to be saved/retrieved.
 * \tparam HARD_LOCATION_BIT_COUNT Bit count of the hard location.
 */
template <
  size_t DATA_BIT_COUNT,
  size_t HARD_LOCATION_BIT_COUNT>
class UpDownCountersFactory :
  public FactoryAbstract<
    UpDownCounters<DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>> {
 public:
  explicit UpDownCountersFactory(FLOAT geometricRatio) {
    this->_instance =
      spUpDownCounters<DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>(
        new UpDownCounters<DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>(
          geometricRatio));
  }
};

}  // namespace sdm
