/*
  This file is part of amm.
  Copyright (C) 2014-2016  Chirantan Mitra <chirantan.mitra@gmail.com>

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

#include <string>
#include <vector>

#include "stringx.h"
#include "filex.h"
#include "directoryx.h"
#include "system_environment.h"
#include "xdg/icon_theme.h"
#include "qualified_icon_theme.h"

namespace amm {

QualifiedIconTheme::QualifiedIconTheme(const SystemEnvironment &environment, const std::string &theme_name) : theme_name_(theme_name)
{
    auto theme_directories = environment.iconThemeDirectories();

    for (const auto &dir : theme_directories) {
        if (FileX(dir).exists()) {
            theme_search_paths_.push_back(dir);
        }
    }
}

xdg::IconTheme QualifiedIconTheme::iconThemeFromName(const std::string &theme_name)
{
    for (const auto &path : theme_search_paths_) {
        DirectoryX directory(path);

        if (directory.isValid()) {
            auto entries = directory.allEntries();
            for (const auto &entry : entries) {
                auto name = entry.name();
                if (entry.isDirectory() && name != "." && name != "..") {
                    auto full_path = StringX(path).terminateWith("/") + StringX(entry.name()).terminateWith("/") + "index.theme";
                    std::vector<std::string> lines;

                    if (FileX(full_path).readLines(&lines)) {
                        auto xdg_theme = xdg::IconTheme(lines).internalNameIs(entry.name());
                        if (xdg_theme.isNamed(theme_name)) {
                            return xdg_theme;
                        }
                    }
                }
            }
        }
    }

    std::vector<std::string> empty_lines;
    return xdg::IconTheme(empty_lines);
}

xdg::IconTheme QualifiedIconTheme::currentIconThemeFromName()
{
    return iconThemeFromName(theme_name_);
}

std::vector<xdg::IconTheme> QualifiedIconTheme::themeWithParent()
{
    std::vector<xdg::IconTheme> icon_themes;
    auto icon_theme = currentIconThemeFromName();
    icon_themes.push_back(icon_theme);

    auto parent_themes = icon_theme.parents();
    for (const auto &theme : parent_themes) {
        icon_themes.push_back(iconThemeFromName(theme));
    }
    return icon_themes;
}

} // namespace amm
