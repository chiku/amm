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

#include "stringx.h"

#include <string>
#include <vector>

#include "catch.hpp"

namespace amm {

SCENARIO("StringX", "[stringx]") {
    GIVEN("A stringx with content") {
        auto stringx = StringX("vlc.desktop");

        WHEN("it has a sub-string at the end") {
            THEN("it ends with the substring") {
                CHECK(stringx.endsWith(".desktop"));
            }

            THEN("it doesn't end with a substring that is not at the end") {
                CHECK_FALSE(stringx.endsWith(".desk"));
            }

            THEN("it doesn't ends with a superstring") {
                CHECK_FALSE(stringx.endsWith("vlc.desktop file"));
            }
        }
    }

    GIVEN("A stringx without terminating delimeters") {
        auto stringx = StringX("AudioVideo;GTK");

        WHEN("terminated with a delimeter") {
            auto result = stringx.terminateWith(";");
            THEN("it appends the delimeter to itself") {
                CHECK(result == "AudioVideo;GTK;");
            }
        }

        WHEN("split by the delimeter") {
            auto result = stringx.split(";");
            THEN("the last token is included") {
                CHECK(result == (std::vector<std::string> { "AudioVideo", "GTK" }));
            }
        }
    }

    GIVEN("A stringx with a terminating delimeters") {
        auto stringx = StringX("AudioVideo;GTK;");

        WHEN("terminated with the delimeter") {
            auto result = stringx.terminateWith(";");
            THEN("it doesn't duplicate the delimeter") {
                CHECK(result == "AudioVideo;GTK;");
            }
        }

        WHEN("split by the delimeter") {
            auto result = stringx.split(";");
            THEN("the original string becomes a list of tokens") {
                CHECK(result == (std::vector<std::string> { "AudioVideo", "GTK" }));
            }
        }
    }

    GIVEN("An empty stringx") {
        auto stringx = StringX("");

        WHEN("terminated with a delimeter") {
            auto result = stringx.terminateWith(";");
            THEN("it appends the delimeter to itself") {
                CHECK(result == ";");
            }
        }

        WHEN("split") {
            auto result = stringx.split(";");
            THEN("it is empty") {
                CHECK(result.empty());
            }
        }
    }

    GIVEN("A stringx with XML tags") {
        auto stringx = StringX("Icon=<\'foo\' & \"bar\">");

        WHEN("XML encoded") {
            auto result = stringx.encode();
            THEN("the XML tags are replaced by corresponding XML escpace sequences") {
                CHECK(result == "Icon=&lt;&apos;foo&apos; &amp; &quot;bar&quot;&gt;");
            }
        }
    }

    GIVEN("A stringx with whitespaces at extremes") {
        auto stringx = StringX(" \taccessories-text-editor \t\n");

        WHEN("trimmed") {
            auto result = stringx.trim();
            THEN("all whitespaces at the beginning and in the end are removed") {
                CHECK(result == "accessories-text-editor");
            }
        }
    }
}

} // namespace amm
