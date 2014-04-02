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

#include <string>
#include <vector>
#include <cstdlib>
#include <dirent.h>

#include "util.h"
#include "desktop_file_names.h"

static std::string
xdg_data_dirs()
{
  char *xdg_data_dirs = std::getenv("XDG_DATA_DIRS");
  if (xdg_data_dirs != NULL) {
    return xdg_data_dirs;
  }

  return "/usr/local/share:/usr/share";
}

static std::string
xdg_data_home()
{
  char *xdg_data_home = std::getenv("XDG_DATA_HOME");
  if (xdg_data_home != NULL) {
    return xdg_data_home;
  }

  char *home = std::getenv("HOME");
  if (home != NULL) {
    return std::string(home) + "/.local/share/applications";
  }

  return "";
}

amm::desktop_file_names::desktop_file_names()
{
  capture_bad_paths_ = true;
}

void
amm::desktop_file_names::register_directories_with_default_fallback(std::vector<std::string> directory_names)
{
  if (directory_names.size() > 0) {
    register_directories(directory_names);
  } else {
    register_default_directories();
  }
}

void
amm::desktop_file_names::register_directories(std::vector<std::string> directory_names)
{
  directory_names_ = directory_names;
}

void
amm::desktop_file_names::register_default_directories()
{
  std::vector<std::string> directory_bases = amm::StringX(xdg_data_dirs()).Split(":");
  directory_bases.push_back(xdg_data_home());

  for (std::vector<std::string>::const_iterator iter = directory_bases.begin(); iter != directory_bases.end(); ++iter) {
    std::string directory = amm::StringX(*iter).TerminateWith("/") + "applications";
    directory_names_.push_back(directory);
  }

  capture_bad_paths_ = false;
}

void
amm::desktop_file_names::resolve()
{
  desktop_file_names_.clear();
  bad_paths_.clear();

  std::vector<std::string> terminated_directory_names = amm::VectorX(directory_names_).TerminateWith("/");
  std::vector<std::string> unique_directory_names = amm::VectorX(terminated_directory_names).Unique();

  std::vector<std::string>::const_iterator name;
  for (name = unique_directory_names.begin(); name != unique_directory_names.end(); ++name) {
    DIR *directory = opendir(name->c_str());

    if (directory) {
      populate(directory, *name);
      closedir(directory);
    } else if (capture_bad_paths_) {
      bad_paths_.push_back(*name);
    }
  }
}

void
amm::desktop_file_names::populate(DIR* directory, std::string directory_name)
{
  dirent *directory_entry;

  while((directory_entry = readdir(directory)) != NULL) {
    std::string file_name = directory_entry->d_name;
    if (amm::StringX(file_name).EndsWith(DESKTOP_EXTENSION)) {
      desktop_file_names_.push_back(directory_name + file_name);
    }
  }
}

std::vector<std::string>
amm::desktop_file_names::all() const
{
  return desktop_file_names_;
}

std::vector<std::string>
amm::desktop_file_names::bad_paths() const
{
  return bad_paths_;
}
