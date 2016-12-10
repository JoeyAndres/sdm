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
#include "AddressRegister.h"

using std::shared_ptr;

namespace sdm {

/*!\class AddressRegisterFactory
 * \brief Factory method for addressRegister
 * \tparam ADDRESS_BIT_COUNT The bit count of the address data.
 * \tparam HARD_LOCATION_BIT_COUNT Hard location bit count.
 */
template<size_t ADDRESS_BIT_COUNT, size_t HARD_LOCATION_BIT_COUNT>
class AddressRegisterFactory :
  public FactoryAbstract<
    AddressRegister<ADDRESS_BIT_COUNT, HARD_LOCATION_BIT_COUNT>> {
 public:
  AddressRegisterFactory() {
    this->_instance = spAddressRegister<ADDRESS_BIT_COUNT,
                                        HARD_LOCATION_BIT_COUNT>(
      new AddressRegister<ADDRESS_BIT_COUNT, HARD_LOCATION_BIT_COUNT>);
  }
};

}  // namespace sdm
