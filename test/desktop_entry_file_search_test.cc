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

#include "desktop_entry_file_search.h"

#include <string>
#include <vector>
#include <algorithm>

#include "catch.hpp"

namespace amm {
namespace service {

static bool present_in(std::string item, std::vector<std::string> list) {
  return std::find(list.begin(), list.end(), item) != list.end();
}

static void assert_files_are_present_in_list(std::vector<std::string> file_names) {
  REQUIRE(file_names.size() == 8);

  REQUIRE(present_in("test/fixtures/applications/missing.desktop", file_names));
  REQUIRE(present_in("test/fixtures/applications/mousepad.desktop", file_names));
  REQUIRE(present_in("test/fixtures/applications/unclassified.desktop", file_names));
  REQUIRE(present_in("test/fixtures/applications/vlc.desktop", file_names));
  REQUIRE(present_in("test/fixtures/applications/suppressed.desktop", file_names));
  REQUIRE(present_in("test/fixtures/applications/suppressedinvalid.desktop", file_names));
  REQUIRE(present_in("test/fixtures/applications/nested/xfburn.desktop", file_names));
  REQUIRE(present_in("test/fixtures/applications/nested/deepnested/whaawmp.desktop", file_names));
}


SCENARIO("service::FileSearch custom directories", "[filesearch]") {
  GIVEN("A file search service with one directory") {
    std::vector<std::string> directory_names;
    directory_names.push_back("test/fixtures/applications/");
    FileSearch service;
    service.RegisterDirectories(directory_names);

    WHEN("resolved") {
      service.Resolve();

      THEN("it has a list of files with extension 'desktop' inside the directory") {
        assert_files_are_present_in_list(service.DesktopEntryFileNames());
      }
    }
  }

  GIVEN("A file search service with repeated directory") {
    std::vector<std::string> directory_names;
    directory_names.push_back("test/fixtures/applications/");
    directory_names.push_back("test/fixtures/applications");
    FileSearch service;
    service.RegisterDirectories(directory_names);

    WHEN("resolved") {
      service.Resolve();

      THEN("it ignores duplicates") {
        assert_files_are_present_in_list(service.DesktopEntryFileNames());
      }
    }
  }

  GIVEN("A file search service with one existing directory and one missing directory") {
    std::vector<std::string> directory_names;
    directory_names.push_back("test/fixtures/applications");
    directory_names.push_back("test/does-not-exist/applications");
    FileSearch service;
    service.RegisterDirectories(directory_names);

    WHEN("resolved") {
      service.Resolve();

      THEN("it has a list of files with extension 'desktop' in the directory that exists") {
        assert_files_are_present_in_list(service.DesktopEntryFileNames());
      }

      THEN("it tracks the directory that doesn't exist") {
        std::vector<std::string> bad_paths = service.BadPaths();
        REQUIRE(bad_paths.size() == 1);
        REQUIRE(bad_paths[0] == "test/does-not-exist/applications/");
      }
    }
  }
}

SCENARIO("service::FileSearch default directories", "[filesearch]") {
  GIVEN("XDG_DATA_DIRS points to a existing directory and a missing directory") {
    unsetenv("HOME");
    unsetenv("XDG_DATA_HOME");
    setenv("XDG_DATA_DIRS", "test/fixtures:test/does-not-exist", 1);

    GIVEN("A default file search service") {
      FileSearch service;
      service.RegisterDefaultDirectories();

      WHEN("resolved") {
        service.Resolve();

        THEN("it has a list of files with extension 'desktop' inside the directory") {
          assert_files_are_present_in_list(service.DesktopEntryFileNames());
        }

        THEN("it doesn't track the absent directory") {
          REQUIRE(service.BadPaths().size() == 0);
        }
      }
    }
  }
}

} // namespace service
} // namespace amm