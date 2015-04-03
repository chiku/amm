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

#include "directoryx.h"

#include <string>
#include <vector>
#include <algorithm>

#include "catch.hpp"

namespace amm {

void assertFileNamesAreCorrect(std::vector<std::string> names)
{
    std::sort(names.begin(), names.end());
    CHECK(names == (std::vector<std::string> {
        "desktop.vlc",
        "missing.desktop",
        "mousepad.desktop",
        "suppressed.desktop",
        "suppressedinvalid.desktop",
        "unclassified.desktop",
        "vlc.desktop",
    }));
}

void assertDirectoryNamesAreCorrect(std::vector<std::string> names)
{
    std::sort(names.begin(), names.end());
    CHECK(names == (std::vector<std::string> {
        ".",
        "..",
        "empty",
        "nested",
    }));
}

void assertNamesAreCorrect(std::vector<std::string> names)
{
    std::sort(names.begin(), names.end());
    CHECK(names == (std::vector<std::string> {
        ".",
        "..",
        "desktop.vlc",
        "empty",
        "missing.desktop",
        "mousepad.desktop",
        "nested",
        "suppressed.desktop",
        "suppressedinvalid.desktop",
        "unclassified.desktop",
        "vlc.desktop",
    }));
}

SCENARIO("DirectoryX", "[directoryx]") {
    GIVEN("A directoryx") {
        WHEN("present") {
            THEN("it is valid") {
                auto directory = DirectoryX("test/fixtures/applications");
                CHECK(directory.isValid());
            }
        }

        WHEN("not present") {
            auto directory = DirectoryX("does-not-exist");
            THEN("it is invalid") {
                CHECK_FALSE(directory.isValid());
            }
        }

        WHEN("it has files under itself") {
            auto directory = DirectoryX("test/fixtures/applications");
            auto entries = directory.allEntries();
            WHEN("its contained files are retrieved one at a time") {
                THEN("the retrieved entry has the name of the file and knows if it is a directory") {
                    auto file_names = std::vector<std::string> {};
                    auto directory_names = std::vector<std::string> {};

                    for (int i = 0; i < 11; i++) {
                        auto result = entries.nextName();
                        CHECK(result.success);
                        (result.isDirectory ? directory_names : file_names).push_back(result.name);
                    }

                    assertFileNamesAreCorrect(file_names);
                    assertDirectoryNamesAreCorrect(directory_names);
                }
            }

            WHEN("its entries are iterated beyond range") {
                THEN("it fails") {
                    for (int i = 0; i < 11; i++) {
                        entries.nextName();
                    }
                    auto result = entries.nextName();

                    CHECK_FALSE(result.success);
                    CHECK_FALSE(result.isDirectory);
                    CHECK(result.name == "");
                }
            }

            WHEN("it is iterated for contained files (with prefix, indirection & not-equals)") {
                THEN("it has the contained file and sub-directory names") {
                    auto file_names = std::vector<std::string> {};
                    auto directory_names = std::vector<std::string> {};
                    for (auto entry = entries.begin(); entry != entries.end(); ++entry) {
                        (entry->isDirectory() ? directory_names : file_names).push_back(entry->name());
                    }

                    assertFileNamesAreCorrect(file_names);
                    assertDirectoryNamesAreCorrect(directory_names);
                }
            }

            WHEN("it is iterated for contained files (with postfix, access to pointer at & equals)") {
                THEN("it has the contained file and sub-directory names") {
                    auto file_names = std::vector<std::string> {};
                    auto directory_names = std::vector<std::string> {};
                    for (auto entry = entries.begin(); !(entry == entries.end()); entry++) {
                        (entry->isDirectory() ? directory_names : file_names).push_back(entry->name());
                    }

                    assertFileNamesAreCorrect(file_names);
                    assertDirectoryNamesAreCorrect(directory_names);
                }
            }
        }

        WHEN("iterated multiple times") {
            THEN("all iterations have the same results") {
                auto directory = DirectoryX("test/fixtures/applications");
                auto first_set = directory.allEntries();
                auto second_set = directory.allEntries();

                auto first_set_names = std::vector<std::string> {};
                for (auto entry = first_set.begin(); entry != first_set.end(); ++entry) {
                    first_set_names.push_back(entry->name());
                }

                auto second_set_names = std::vector<std::string> {};
                for (auto entry = second_set.begin(); entry != second_set.end(); ++entry) {
                    second_set_names.push_back(entry->name());
                }

                assertNamesAreCorrect(first_set_names);
                assertNamesAreCorrect(second_set_names);
            }
        }
    }
}

} // namespace amm
