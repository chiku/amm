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

#ifndef AMM_TRANSFORMER_JWM_H_
#define AMM_TRANSFORMER_JWM_H_

#include <string>

#include "representation.h"
#include "transformer.h"

namespace amm {
namespace transformer {
// Understands the conversion of representations into a JWM menu
class Jwm : public TransformerInterface {
 public:
  virtual std::string Transform(const representation::MenuStart &entry) const;
  virtual std::string Transform(const representation::MenuEnd &entry) const;
  virtual std::string Transform(const representation::SubcategoryStart &entry) const;
  virtual std::string Transform(const representation::SubcategoryEnd &entry) const;
  virtual std::string Transform(const representation::Program &entry) const;
};
} // namespace transformer
} // namespace amm

#endif // AMM_TRANSFORMER_JWM_H_
