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

#include "xdg/entry_line.h"

#include <vector>
#include <string>

#include "../catch.hpp"

namespace amm {
namespace xdg {

SCENARIO("EntryLine for Comments", "[entryline]") {
    GIVEN("A line in a desktop-file") {
        WHEN("starting with a '#'") {
            auto line = EntryLine("# Comment line");

            THEN("it is not a declaration") { CHECK_FALSE(line.isDeclaration()); }
            THEN("it has no declarations") { CHECK(line.declaration() == ""); }
            THEN("it is not an assignment") { CHECK_FALSE(line.isAssignment()); }
            THEN("it has no key") { CHECK(line.key() == ""); }
            THEN("it has no value") { CHECK(line.value() == ""); }
        }
    }
}

SCENARIO("EntryLine for Declarations", "[entryline]") {
    GIVEN("A line in a desktop-file") {
        WHEN("starting with a '[' and ending with ']'") {
            auto line = EntryLine("[Desktop Entry]");

            THEN("it is a declaration") { CHECK(line.isDeclaration()); }
            THEN("the inner content is the declaration") { CHECK(line.declaration() == "Desktop Entry"); }
            THEN("it is not an assignment") { CHECK_FALSE(line.isAssignment()); }
            THEN("it has no key") { CHECK(line.key() == ""); }
            THEN("it has no value") { CHECK(line.value() == ""); }
        }

        WHEN("it has spaces around after a declaration") {
            auto line = EntryLine(" [Desktop Entry] ");
            THEN("the spaces are ignored") { CHECK(line.declaration() == "Desktop Entry"); }
        }

        WHEN("starting with a '[' but not ending with ']'") {
            auto line = EntryLine("[Desktop Entry");
            THEN("it is not a declaration") { CHECK_FALSE(line.isDeclaration()); }
        }
    }
}

SCENARIO("EntryLine for Assignments", "[entryline]") {
    GIVEN("A line in a desktop-file") {
        WHEN("it has an '=' in the middle") {
            auto line = EntryLine("Name=VLC");

            THEN("it is an assignment") { CHECK(line.isAssignment()); }
            THEN("it has a key") { CHECK(line.key() == "Name"); }
            THEN("it has a value") { CHECK(line.value() == "VLC"); }

            THEN("it is not a declaration") { CHECK_FALSE(line.isDeclaration()); }
            THEN("it has no declarations") { CHECK(line.declaration() == ""); }
        }

        WHEN("it has spaces surrounding the tokens") {
            auto line = EntryLine("\t Name = VLC \n\n");
            THEN("the spaces are ignored") {
                CHECK(line.key() == "Name");
                CHECK(line.value() == "VLC");
            }
        }
    }
}

} // namespace xdg
} // namespace amm
