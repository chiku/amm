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

#include "desktop_entry_file_search.h"

#include <string>
#include <vector>
#include <algorithm>

#include "catch.hpp"

namespace amm {

static void assertFilesArePresentInList(std::vector<std::string> file_names) {
    std::sort(file_names.begin(), file_names.end());
    CHECK(file_names == (std::vector<std::string> {
        "test/fixtures/applications/missing.desktop",
        "test/fixtures/applications/mousepad.desktop",
        "test/fixtures/applications/nested/deepnested/whaawmp.desktop",
        "test/fixtures/applications/nested/xfburn.desktop",
        "test/fixtures/applications/suppressed.desktop",
        "test/fixtures/applications/suppressedinvalid.desktop",
        "test/fixtures/applications/unclassified.desktop",
        "test/fixtures/applications/vlc.desktop",
    }));
}


SCENARIO("DesktopEntryFileSearch custom directories", "[filesearch]") {
    GIVEN("A file search service with one directory") {
        auto directory_names = std::vector<std::string> { "test/fixtures/applications/" };
        auto searcher = DesktopEntryFileSearch();
        searcher.registerDirectories(directory_names);

        WHEN("resolved") {
            searcher.resolve();

            THEN("it has a list of files with extension 'desktop' inside the directory") {
                assertFilesArePresentInList(searcher.desktopEntryFileNames());
            }
        }
    }

    GIVEN("A file search service with repeated directory") {
        auto directory_names = std::vector<std::string> { "test/fixtures/applications/", "test/fixtures/applications" };
        auto searcher = DesktopEntryFileSearch();
        searcher.registerDirectories(directory_names);

        WHEN("resolved") {
            searcher.resolve();

            THEN("it ignores the duplicates") {
                assertFilesArePresentInList(searcher.desktopEntryFileNames());
            }
        }
    }

    GIVEN("A file search service with one existing directory and one missing directory") {
        auto directory_names = std::vector<std::string> { "test/fixtures/applications", "test/does-not-exist/applications" };
        auto searcher = DesktopEntryFileSearch();
        searcher.registerDirectories(directory_names);

        WHEN("resolved") {
            searcher.resolve();

            THEN("it has a list of files with extension 'desktop' inside the existing directory") {
                assertFilesArePresentInList(searcher.desktopEntryFileNames());
            }

            THEN("it tracks the missing directory") {
                CHECK(searcher.badPaths() == (std::vector<std::string> {"test/does-not-exist/applications/"}));
            }
        }
    }
}

SCENARIO("DesktopEntryFileSearch default directories", "[filesearch]") {
    GIVEN("XDG_DATA_DIRS points to a existing directory and a missing directory") {
        unsetenv("HOME");
        unsetenv("XDG_DATA_HOME");
        setenv("XDG_DATA_DIRS", "test/fixtures:test/does-not-exist", 1);

        GIVEN("A default file search service") {
            auto searcher = DesktopEntryFileSearch();
            searcher.registerDefaultDirectories();

            WHEN("resolved") {
                searcher.resolve();

                THEN("it has a list of files with extension 'desktop' inside the directory") {
                    assertFilesArePresentInList(searcher.desktopEntryFileNames());
                }

                THEN("it doesn't track the absent directory") {
                    CHECK(searcher.badPaths().empty());
                }
            }
        }
    }
}

} // namespace amm
