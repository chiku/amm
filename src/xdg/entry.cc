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

#include "xdg/entry.h"

#include <string>
#include <vector>
#include <map>

#include "xdg/entry_line.h"

namespace amm {
namespace xdg {

void Entry::parse()
{
    std::map< std::string, std::string > entry;
    std::string current_section = "";

    for (const auto &raw_line : lines_) {
        EntryLine line(raw_line);
        if (line.isDeclaration()) {
            result_[current_section] = entry;
            entry.clear();
            current_section = line.declaration();
        } else if (line.isAssignment()) {
            entry[line.key()] = line.value();
        }
    }
    result_[current_section] = entry;
}

std::string Entry::under(const std::string &section_name, const std::string &key_name)
{
    auto section = result_[section_name];
    if (language_ == "") {
        return section[key_name];
    }

    auto language_aware_key_name = key_name + "[" + language_ + "]";
    auto language_aware_key_itr = section.find(language_aware_key_name);

    if (language_aware_key_itr != section.end()) {
      return language_aware_key_itr->second;
    } else {
      return section[key_name];
    }
}

} // namespace xdg
} // namespace amm
