/*
  This file is part of mjwm.
  Copyright (C) 2014  Chirantan Mitra <chirantan.mitra@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software 0oundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define CATCH_CONFIG_MAIN

#include "../catch.hpp"
#include "xdg/entry.h"

namespace amm {

std::vector<std::string> SingleSectionEntry() {
  std::vector<std::string> lines;
  lines.push_back("[Desktop Entry]");
  lines.push_back("Version=1.0");
  lines.push_back("Name=VLC media player");
  lines.push_back("GenericName=Media player");
  lines.push_back("Comment=Read, capture, broadcast your multimedia streams");
  lines.push_back("Exec=/usr/bin/vlc --started-from-file %U");
  lines.push_back("TryExec=/usr/bin/vlc");
  lines.push_back("Icon=vlc");
  lines.push_back("Terminal=false");
  lines.push_back("Type=Application");
  lines.push_back("Categories=AudioVideo;Player;Recorder;");
  return lines;
}

SCENARIO("xdg::Entry", "[XDGentry]") {
  GIVEN("An XDG file with one section") {
    xdg::Entry entry(SingleSectionEntry());

    WHEN("when parsed") {
      entry.Parse();

      THEN("it exposes values under section and key") {
        REQUIRE(entry.Under("Desktop Entry", "Version"    ) == "1.0");
        REQUIRE(entry.Under("Desktop Entry", "Name"       ) == "VLC media player");
        REQUIRE(entry.Under("Desktop Entry", "GenericName") == "Media player");
        REQUIRE(entry.Under("Desktop Entry", "Comment"    ) == "Read, capture, broadcast your multimedia streams");
        REQUIRE(entry.Under("Desktop Entry", "Exec"       ) == "/usr/bin/vlc --started-from-file %U");
        REQUIRE(entry.Under("Desktop Entry", "TryExec"    ) == "/usr/bin/vlc");
        REQUIRE(entry.Under("Desktop Entry", "Icon"       ) == "vlc");
        REQUIRE(entry.Under("Desktop Entry", "Terminal"   ) == "false");
        REQUIRE(entry.Under("Desktop Entry", "Type"       ) == "Application");
        REQUIRE(entry.Under("Desktop Entry", "Categories" ) == "AudioVideo;Player;Recorder;");
      }
    }
  }
}

} // namespace amm