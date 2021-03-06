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

#include "system_environment.h"

#include <cstdlib>
#include <string>
#include <vector>

#include "stringx.h"

namespace amm {

SystemEnvironment::SystemEnvironment()
{
    auto *home = std::getenv("HOME");
    if (home != nullptr) {
        home_ = home;
    }

    auto *xdg_data_home = std::getenv("XDG_DATA_HOME");
    if (xdg_data_home != nullptr) {
        xdg_data_home_ = xdg_data_home;
    } else {
        xdg_data_home_ = home_ + "/.local/share";
    }

    auto *xdg_data_dirs = std::getenv("XDG_DATA_DIRS");
    if (xdg_data_dirs != nullptr) {
        xdg_data_dirs_ = xdg_data_dirs;
    } else {
        xdg_data_dirs_ = "/usr/local/share:/usr/share";
    }

    language_ = getLanguageWith(std::getenv("LANGUAGE"));
    if (language_ == "")  {
        language_ = getLanguageWith(std::getenv("LC_ALL"));
    }
    if (language_ == "") {
        language_ = getLanguageWith(std::getenv("LANG"));
    }
}

std::vector<std::string> SystemEnvironment::xdgDataDirectories() const
{
    return StringX(xdg_data_dirs_).split(":");
}

std::vector<std::string> SystemEnvironment::applicationDirectories() const
{
    std::vector<std::string> directories;
    auto home_application = StringX(xdgDataHome()).terminateWith("/") + "applications";
    directories.push_back(home_application);

    auto data_dir_applications = xdgDataDirectories();
    for (const auto &directory : data_dir_applications) {
        directories.push_back(StringX(directory).terminateWith("/") + "applications");
    }
    return directories;
}

std::vector<std::string> SystemEnvironment::iconThemeDirectories() const
{
    std::vector<std::string> directories;
    auto home_icon = StringX(home()).terminateWith("/") + ".icons";
    directories.push_back(home_icon);

    auto xdg_data_dirs = xdgDataDirectories();
    for (const auto &directory : xdg_data_dirs) {
        directories.push_back(StringX(directory).terminateWith("/") + "icons");
    }
    directories.push_back("/usr/share/pixmaps");

    return directories;
}

std::string SystemEnvironment::getLanguageWith(const char *raw)
{
    if (raw == nullptr) {
        return "";
    }

    auto without_encoding = StringX(raw).split(".")[0];
    return StringX(without_encoding).split("_")[0];
}

} // namespace amm
