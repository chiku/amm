/*
  This file is part of mjwm.
  Copyright (C) 2014  Chirantan Mitra <chirantan.mitra@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AMM_TRANSFORMER_H_
#define AMM_TRANSFORMER_H_

#include <string>

#include "representation.h"

namespace amm {
namespace representation {
  // Forward declare classes to break circular dependency between amm::representation::* and amm::transformer::*
  class MenuStart;
  class MenuEnd;
  class SubcategoryStart;
  class SubcategoryEnd;
  class Program;
} // namespace representation

class TransformerInterface {
 public:
  // TODO : these Transform functions should use a reference instead of a pointer
  virtual std::string Transform(const representation::MenuStart &entry) const = 0;
  virtual std::string Transform(const representation::MenuEnd &entry) const = 0;
  virtual std::string Transform(const representation::SubcategoryStart &entry) const = 0;
  virtual std::string Transform(const representation::SubcategoryEnd &entry) const = 0;
  virtual std::string Transform(const representation::Program &entry) const = 0;
};
} // namespace amm

#endif // AMM_TRANSFORMER_H_