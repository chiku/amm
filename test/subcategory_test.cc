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

#include "subcategory.h"

#include <string>
#include <vector>

#include "catch.hpp"
#include "xdg/desktop_entry.h"

namespace amm {

static xdg::DesktopEntry mousepadDesktopEntry() {
    auto entry = xdg::DesktopEntry();
    entry.parse(std::vector<std::string> {
        "[Desktop Entry]",
        "Name=Mousepad",
        "Icon=accessories-text-editor",
        "Exec=mousepad %F",
        "Categories=Application;Utility;TextEditor;GTK;",
    });
    return entry;
}

static xdg::DesktopEntry sakuraDesktopEntry() {
    auto entry = xdg::DesktopEntry();
    entry.parse(std::vector<std::string> {
        "[Desktop Entry]",
        "Name=Sakura",
        "Icon=terminal-tango",
        "Exec=sakura",
        "Categories=GTK;Utility;TerminalEmulator;System;",
    });
    return entry;
}

SCENARIO("subcategory", "[subcategory]") {
    GIVEN("A subcategory") {
        Subcategory subcategory("Accessories", "accessories", "Utilities");

        WHEN("without desktop-files") {
            THEN("it has no entries") {
                CHECK_FALSE(subcategory.hasEntries());
            }
        }

        WHEN("with one desktop-file") {
            subcategory.addDesktopEntry(mousepadDesktopEntry());
            THEN("it has entries") {
                CHECK(subcategory.hasEntries());
            }
        }

        WHEN("with two desktop-file") {
            subcategory.addDesktopEntry(sakuraDesktopEntry());
            subcategory.addDesktopEntry(mousepadDesktopEntry());
            WHEN("sorted") {
                subcategory.sortDesktopEntries();
                THEN("its entries are alphabetically sorted by name") {
                    std::vector<xdg::DesktopEntry> desktop_entries = subcategory.desktopEntries();
                    REQUIRE(desktop_entries.size() == 2);
                    CHECK(desktop_entries[0].name() == "Mousepad");
                    CHECK(desktop_entries[1].name() == "Sakura");
                }
            }
        }

        WHEN("with a repeated desktop file") {
            subcategory.addDesktopEntry(sakuraDesktopEntry());
            subcategory.addDesktopEntry(mousepadDesktopEntry());
            subcategory.addDesktopEntry(sakuraDesktopEntry());
            WHEN("sorted") {
                subcategory.sortDesktopEntries();
                THEN("it doesn't repeat entries") {
                    std::vector<xdg::DesktopEntry> desktop_entries = subcategory.desktopEntries();
                    REQUIRE(desktop_entries.size() == 2);
                    CHECK(desktop_entries[0].name() == "Mousepad");
                    CHECK(desktop_entries[1].name() == "Sakura");
                }
            }
        }
    }
}

} // namespace amm
