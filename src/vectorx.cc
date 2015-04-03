/*
  This file is part of amm.
  Copyright (C) 2014-2015  Chirantan Mitra <chirantan.mitra@gmail.com>

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

#include "vectorx.h"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "stringx.h"

namespace amm {

std::string VectorX::join(const std::string &delimeter) const
{
    std::stringstream stream;
    auto vector_size = vector_.size();

    if (vector_size == 0) {
        return "";
    }

    if (vector_size >= 2) {
        for (auto item = vector_.begin(); item != vector_.end()-1; ++item) {
            stream << *item << delimeter;
        }
    }
    stream << vector_.back();

    return stream.str();
}

std::vector<std::string> VectorX::terminateEachWith(const std::string &delimiter) const
{
    std::vector<std::string> result;

    for (const auto &item : vector_) {
        result.push_back(StringX(item).terminateWith(delimiter));
    }

    return result;
}

std::vector<std::string> VectorX::unique() const
{
    auto result = vector_;

    std::sort(result.begin(), result.end());
    auto it = std::unique(result.begin(), result.end());
    result.resize(std::distance(result.begin(), it));

    return result;
}

} // namespace amm
