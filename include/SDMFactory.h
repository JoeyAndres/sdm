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

#include<memory>

#include "./utility/FactoryAbstract.h"
#include "./declares.h"
#include "AddressRegister.h"
#include "AddressRegisterFactory.h"
#include "SDM.h"
#include "UpDownCounters.h"

using std::shared_ptr;

namespace sdm {

/*!\class SDMFactory
 * \brief Factory for sdm.
 * \tparam ADDRESS_BIT_COUNT The bit count of the address data.
 * \tparam HARD_LOCATION_BIT_COUNT Hard location bit count.
 * \tparam DATA_BIT_COUNT Number of bits in the data to be saved.
 *                        Defaults to ADDRESS_BIT_COUNT as in Mr. Karneva's paper.
 */
template <
  size_t ADDRESS_BIT_COUNT,
  size_t HARD_LOCATION_BIT_COUNT,
  size_t DATA_BIT_COUNT = ADDRESS_BIT_COUNT>
class SDMFactory :
  public FactoryAbstract<
    SDM<
      ADDRESS_BIT_COUNT,
      HARD_LOCATION_BIT_COUNT,
      DATA_BIT_COUNT>>{
 public:
  explicit SDMFactory(size_t threshold, FLOAT commonRatio = 0.01F) {
    auto addressRegister =
      sdm::AddressRegisterFactory<
        ADDRESS_BIT_COUNT, HARD_LOCATION_BIT_COUNT>().get();
    auto upDownCounters =
      sdm::UpDownCountersFactory<
        DATA_BIT_COUNT, HARD_LOCATION_BIT_COUNT>(commonRatio).get();
    this->_instance =
      spSDM<
        ADDRESS_BIT_COUNT,
        HARD_LOCATION_BIT_COUNT,
        DATA_BIT_COUNT>(
        new SDM<
          ADDRESS_BIT_COUNT,
          HARD_LOCATION_BIT_COUNT,
          DATA_BIT_COUNT>(addressRegister, upDownCounters, threshold));
  }
};

}  // namespace sdm
