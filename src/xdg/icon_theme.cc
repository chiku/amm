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

#include "xdg/icon_theme.h"

#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

#include "stringx.h"
#include "xdg/entry.h"
#include "xdg/icon_subdirectory.h"

namespace amm {
namespace xdg {

IconTheme::IconTheme(const std::vector<std::string> &lines) : internal_name_("")
{
    Entry xdg_entry(lines);
    xdg_entry.parse();

    name_ = xdg_entry.under("Icon Theme", "Name");
    auto lower_case_name = name_;
    std::transform(lower_case_name.begin(), lower_case_name.end(), lower_case_name.begin(), ::tolower);

    parents_ = StringX(xdg_entry.under("Icon Theme", "Inherits")).split(",");
    if (parents_.empty() && lower_case_name != "hicolor") {
        parents_.push_back("Hicolor");
    }

    auto directory_names = StringX(xdg_entry.under("Icon Theme", "Directories")).split(",");
    for (const auto &name : directory_names) {
        auto type = xdg_entry.under(name, "Type");
        auto size = xdg_entry.under(name, "Size");
        auto maxsize = xdg_entry.under(name, "MaxSize");
        auto minsize = xdg_entry.under(name, "MinSize");
        auto threshold = xdg_entry.under(name, "Threshold");

        auto icon_subdirectory = IconSubdirectory(name, size)
            .type(type)
            .maxSize(maxsize)
            .minSize(minsize)
            .threshold(threshold);
        directories_.push_back(icon_subdirectory);
    }
}

} // namespace xdg
} // namespace amm
