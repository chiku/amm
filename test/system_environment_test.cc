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

#define CATCH_CONFIG_MAIN

#include "system_environment.h"

#include <cstdlib>
#include <string>
#include <vector>

#include "catch.hpp"

namespace amm {

SCENARIO("SystemEnvironment", "[systemenvironment]") {
  GIVEN("XDG_DATA_HOME and XDG_DATA_DIRS are set") {
    setenv("XDG_DATA_HOME", "/data/home", 1);
    setenv("XDG_DATA_DIRS", "/data/dir1:/data/dir2/", 1);

    SystemEnvironment environment;

    WHEN("XDG data home is asked") {
      THEN("it is the directory pointed to by XDG_DATA_HOME") {
        REQUIRE(environment.XdgDataHome() == "/data/home");
      }
    }

    WHEN("XDG data directories is asked") {
      THEN("it is a list of directories pointed to by XDG_DATA_DIRS") {
        std::vector<std::string> directories = environment.XdgDataDirectories();
        REQUIRE(directories.size() == 2);
        REQUIRE(directories[0] == "/data/dir1");
        REQUIRE(directories[1] == "/data/dir2/");
      }
    }

    WHEN("directories for 'applications' subdirectories is asked") {
      THEN("it is a list of directories pointed to by XDG_DATA_HOME/applications, XDG_DATA_DIRS/applications") {
        std::vector<std::string> directories = environment.ApplicationDirectories();
        REQUIRE(directories.size() == 3);
        REQUIRE(directories[0] == "/data/home/applications");
        REQUIRE(directories[1] == "/data/dir1/applications");
        REQUIRE(directories[2] == "/data/dir2/applications");
      }
    }
  }

  GIVEN("XDG_DATA_HOME is unset and XDG_DATA_DIRS is unset") {
    setenv("HOME", "/home", 1);
    unsetenv("XDG_DATA_HOME");
    unsetenv("XDG_DATA_DIRS");

    SystemEnvironment environment;

    WHEN("directories for 'applications' subdirectories is asked") {

      THEN("it is a list of directories pointed to by $HOME/.local/share/applications, /usr/local/share/applications and /usr/share/applications") {
        std::vector<std::string> directories = environment.ApplicationDirectories();
        REQUIRE(directories.size() == 3);
        REQUIRE(directories[0] == "/home/.local/share/applications");
        REQUIRE(directories[1] == "/usr/local/share/applications");
        REQUIRE(directories[2] == "/usr/share/applications");
      }
    }

    WHEN("directories for icon themes is asked") {
      THEN("it is a list of directories pointed to by $HOME/.icons, /usr/local/share/icons, /usr/share/icons and /usr/share/pixmaps") {
        std::vector<std::string> directories = environment.IconThemeDirectories();
        REQUIRE(directories.size() == 4);
        REQUIRE(directories[0] == "/home/.icons");
        REQUIRE(directories[1] == "/usr/local/share/icons");
        REQUIRE(directories[2] == "/usr/share/icons");
        REQUIRE(directories[3] == "/usr/share/pixmaps");
      }
    }
  }
}

} // namespace amm