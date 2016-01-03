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

#include "vectorx.h"

#include <string>
#include <vector>

#include "catch.hpp"

namespace amm {

SCENARIO("VectorX", "[vectorx]") {
    GIVEN("A VectorX with more than one item") {
        auto vector = std::vector<std::string> { "foo", "bar", "baz" };
        auto vectorx = VectorX(vector);

        WHEN("joined with a delimeter") {
            auto result = vectorx.join("; ");
            THEN("it is a string with the delimeter in between the items") {
                CHECK(result == "foo; bar; baz");
            }
        }
    }

    GIVEN("A vectorx with one item") {
        auto vector = std::vector<std::string> { "foo" };
        auto vectorx = VectorX(vector);

        WHEN("joined with a delimeter") {
            auto result = vectorx.join("; ");
            THEN("it the item in the original vector") {
                CHECK(result == "foo");
            }
        }
    }

    GIVEN("A vectorx with no items") {
        auto vector = std::vector<std::string> {};
        auto vectorx = VectorX(vector);

        WHEN("joined with a delimeter") {
            auto result = vectorx.join("; ");
            THEN("it is an empty string") {
                CHECK(result == "");
            }
        }
    }

    GIVEN("A vectorx with some items ending in a delimeter") {
        auto vector = std::vector<std::string> { "foo/", "bar/", "baz/" };
        auto vectorx = VectorX(vector);

        WHEN("terminated with a delimeter") {
            auto result = vectorx.terminateEachWith("/");
            THEN("it adds the delimeter without duplicating it") {
                CHECK(result == (std::vector<std::string> {"foo/", "bar/", "baz/"}));
            }
        }
    }

    GIVEN("A vectorx with duplicates") {
        auto vector = std::vector<std::string> { "foo", "bar", "foo", "bar", "bar" };
        auto vectorx = VectorX(vector);

        WHEN("unique") {
            auto result = vectorx.unique();
            THEN("it de-duplicates") {
                CHECK(result == (std::vector<std::string> {"bar", "foo"}));
            }
        }
    }
}

} // namespace amm
