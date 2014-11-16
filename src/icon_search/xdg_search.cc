/*
  This file is part of amm.
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

#include "icon_search/xdg_search.h"

#include <climits>
#include <string>
#include <vector>

#include "stringx.h"
#include "filex.h"
#include "xdg/icon_subdirectory.h"
#include "xdg/icon_theme.h"
#include "qualified_icon_theme.h"

namespace amm {
namespace icon_search {

class Path
{
public:
    explicit Path(const std::string &name) { name_ = name; name_.reserve(256); }
    void join(const std::string &path) { name_ += "/"; name_ += path; }
    std::string result() { return name_; }
private:
    std::string name_;
};

XdgSearch::XdgSearch(int size, QualifiedIconTheme qualified_icon_theme) : size_(size)
{
    registered_extensions_.push_back(".png");
    registered_extensions_.push_back(".svg");
    registered_extensions_.push_back(".xpm");
    theme_search_paths_ = qualified_icon_theme.themeSearchPaths();
    icon_themes_ = qualified_icon_theme.themeWithParent();
}

std::string XdgSearch::resolvedName(const std::string &icon_name) const
{
    auto file_name = nameInTheme(icon_name);
    if (file_name != "") {
        return file_name;
    }

    file_name = fallbackName(icon_name);
    if (file_name != "") {
        return file_name;
    }

    return icon_name;
}

std::string XdgSearch::nameInTheme(const std::string &icon_name) const
{
    auto search_locations = findSearchLocations(icon_name);
    auto file_name = lookupBySize(search_locations);

    return file_name;
}

std::vector<xdg::IconSubdirectory> XdgSearch::findSearchLocations(const std::string &icon_name) const
{
    std::vector<xdg::IconSubdirectory> search_locations;

    for (const auto &icon_theme : icon_themes_) {
        auto theme_subdirs = icon_theme.directories();

        for (auto &subdir : theme_subdirs) {
            for (const auto &search_path : theme_search_paths_) {
                for (const auto &extension : registered_extensions_) {
                    Path path(search_path);
                    path.join(icon_theme.internalName());
                    path.join(subdir.name());
                    path.join(icon_name);
                    auto file_name = StringX(path.result()).terminateWith(extension);
                    if (FileX(file_name).exists()) {
                        search_locations.push_back(xdg::IconSubdirectory(subdir.location(file_name)));
                    }
                }
            }
        }
    }

    return search_locations;
}

std::string XdgSearch::lookupBySize(const std::vector<xdg::IconSubdirectory> &search_locations) const
{
    for (auto subdir : search_locations) {
        if (subdir.matches(size_)) {
            return subdir.location();
        }
    }

    auto minimal_size = INT_MAX;
    auto closest_file_name = std::string { "" };
    for (const auto &subdir : search_locations) {
        auto distance = subdir.distance(size_);
        if (distance < minimal_size) {
            closest_file_name = subdir.location();
            minimal_size = distance;
        }
    }

    return closest_file_name;
}

std::string XdgSearch::fallbackName(const std::string &icon_name) const
{
    for (const auto &directory : theme_search_paths_) {
        for (auto extension : registered_extensions_) {
            Path path(directory);
            path.join(icon_name);
            std::string file_name = StringX(path.result()).terminateWith(extension);
            if (FileX(file_name).exists()) {
                return file_name;
            }
        }
    }

    return "";
}

} // namespace icon_search
} // namespace amm
