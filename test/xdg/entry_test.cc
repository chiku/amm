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

#include "../catch.hpp"
#include "xdg/entry.h"

namespace amm {
namespace xdg {

std::vector<std::string> singleSectionEntryLines()
{
    return std::vector<std::string> {
        "[Desktop Entry]",
        "Version=1.0",
        "Name=VLC media player",
        "GenericName=Media player",
        "Comment=Read, capture, broadcast your multimedia streams",
        "Exec=/usr/bin/vlc --started-from-file %U",
        "TryExec=/usr/bin/vlc",
        "Icon=vlc",
        "Terminal=false",
        "Type=Application",
        "Categories=AudioVideo;Player;Recorder;",
    };
}

std::vector<std::string> singleSectionEntryLinesWithLanguages()
{
    return std::vector<std::string> {
        "[Desktop Entry]",
        "Version=1.0",
        "Name=VLC media player",
        "GenericName=Media player",
        "Comment=Read, capture, broadcast your multimedia streams",
        "Name[bn]=VLC মিডিয়া প্লেয়ার",
        "Comment[bn]=আপনার মাল্টিমিডিয়া স্ট্রীম পড়ুন, ধরে রাখুন এবং ছড়িয়ে দিন",
        "Name[br]=VLC lenner mediaoù",
        "GenericName[br]=Lenner mediaoù",
        "Comment[br]=Lenn, enrollañ, skignañ ho lanvioù liesvedia",
        "Exec=/usr/bin/vlc --started-from-file %U",
        "TryExec=/usr/bin/vlc",
        "Icon=vlc",
        "Terminal=false",
        "Type=Application",
        "Categories=AudioVideo;Player;Recorder;",
    };
}

std::vector<std::string> multipleSectionsEntry()
{
    return std::vector<std::string> {
        "[Desktop Entry]",
        "Version=1.0",
        "Type=Application",
        "Name=Xfburn",
        "Comment=CD and DVD burning application",
        "",
        "Exec=xfburn",
        "TryExec=xfburn",
        "Icon=media-cdrom",
        "MimeType=application/x-cd-image;",
        "Terminal=false",
        "Categories=X-XFCE;GTK;DiscBurning;Archiving;AudioVideo;Utility;",
        "StartupNotify=true",
        "",
        "Actions=BurnImage;",
        "",
        "[Desktop Action BurnImage]",
        "Icon=media-cdrom",
        "Exec=xfburn -i %f",
        "Name=Burn Image (xfburn)",
    };
}

std::vector<std::string> noDeclarationLines()
{
    return std::vector<std::string> {
        "Name=VLC media player",
        "Exec=/usr/bin/vlc --started-from-file %U",
        "Icon=vlc",
        "Categories=AudioVideo;Player;Recorder;",
    };
}

std::vector<std::string> singleSectionWithWhiteSpacesLines()
{
    return std::vector<std::string> {
        " [Desktop Entry] ",
        " Name = VLC media player ",
        "Exec=/usr/bin/vlc --started-from-file %U\n\n",
        " Icon=vlc",
        "Categories =AudioVideo;Player;Recorder;\n",
    };
}

std::vector<std::string> singleSectionWithMissingAssignmentLines()
{
    return std::vector<std::string> {
        "[Desktop Entry]",
        "Name=VLC media player",
        "Exec=",
        "=vlc",
        "Categories=",
    };
}

SCENARIO("xdg::Entry", "[XDGentry]") {
    GIVEN("An XDG file with one section") {
        auto entry = xdg::Entry(singleSectionEntryLines());

        WHEN("when parsed") {
            entry.parse();

            THEN("values are exposed under the section and key name") {
                CHECK(entry.under("Desktop Entry", "Version"    ) == "1.0");
                CHECK(entry.under("Desktop Entry", "Name"       ) == "VLC media player");
                CHECK(entry.under("Desktop Entry", "GenericName") == "Media player");
                CHECK(entry.under("Desktop Entry", "Comment"    ) == "Read, capture, broadcast your multimedia streams");
                CHECK(entry.under("Desktop Entry", "Exec"       ) == "/usr/bin/vlc --started-from-file %U");
                CHECK(entry.under("Desktop Entry", "TryExec"    ) == "/usr/bin/vlc");
                CHECK(entry.under("Desktop Entry", "Icon"       ) == "vlc");
                CHECK(entry.under("Desktop Entry", "Terminal"   ) == "false");
                CHECK(entry.under("Desktop Entry", "Type"       ) == "Application");
                CHECK(entry.under("Desktop Entry", "Categories" ) == "AudioVideo;Player;Recorder;");
            }
        }
    }

    GIVEN("An XDG file for an existing language") {
        auto entry = xdg::Entry(singleSectionEntryLinesWithLanguages(), "bn");

        WHEN("when parsed") {
            entry.parse();

            THEN("values are exposed for the language") {
                CHECK(entry.under("Desktop Entry", "Version"    ) == "1.0");
                CHECK(entry.under("Desktop Entry", "Name"       ) == "VLC মিডিয়া প্লেয়ার");
                CHECK(entry.under("Desktop Entry", "GenericName") == "Media player");
                CHECK(entry.under("Desktop Entry", "Comment"    ) == "আপনার মাল্টিমিডিয়া স্ট্রীম পড়ুন, ধরে রাখুন এবং ছড়িয়ে দিন");
                CHECK(entry.under("Desktop Entry", "Exec"       ) == "/usr/bin/vlc --started-from-file %U");
                CHECK(entry.under("Desktop Entry", "TryExec"    ) == "/usr/bin/vlc");
                CHECK(entry.under("Desktop Entry", "Icon"       ) == "vlc");
                CHECK(entry.under("Desktop Entry", "Terminal"   ) == "false");
                CHECK(entry.under("Desktop Entry", "Type"       ) == "Application");
                CHECK(entry.under("Desktop Entry", "Categories" ) == "AudioVideo;Player;Recorder;");
            }
        }
    }

    GIVEN("An XDG file for a non-existing language") {
        auto entry = xdg::Entry(singleSectionEntryLinesWithLanguages(), "bad");

        WHEN("when parsed") {
            entry.parse();

            THEN("default values are exposed") {
                CHECK(entry.under("Desktop Entry", "Version"    ) == "1.0");
                CHECK(entry.under("Desktop Entry", "Name"       ) == "VLC media player");
                CHECK(entry.under("Desktop Entry", "GenericName") == "Media player");
                CHECK(entry.under("Desktop Entry", "Comment"    ) == "Read, capture, broadcast your multimedia streams");
                CHECK(entry.under("Desktop Entry", "Exec"       ) == "/usr/bin/vlc --started-from-file %U");
                CHECK(entry.under("Desktop Entry", "TryExec"    ) == "/usr/bin/vlc");
                CHECK(entry.under("Desktop Entry", "Icon"       ) == "vlc");
                CHECK(entry.under("Desktop Entry", "Terminal"   ) == "false");
                CHECK(entry.under("Desktop Entry", "Type"       ) == "Application");
                CHECK(entry.under("Desktop Entry", "Categories" ) == "AudioVideo;Player;Recorder;");
            }
        }
    }

    GIVEN("An XDG file with entries that contain whitespaces") {
        auto entry = xdg::Entry(singleSectionWithWhiteSpacesLines());

        WHEN("when parsed") {
            entry.parse();

            THEN("the whitespaces are ignored when retrieving the values") {
                CHECK(entry.under("Desktop Entry", "Name"      ) == "VLC media player");
                CHECK(entry.under("Desktop Entry", "Exec"      ) == "/usr/bin/vlc --started-from-file %U");
                CHECK(entry.under("Desktop Entry", "Icon"      ) == "vlc");
                CHECK(entry.under("Desktop Entry", "Categories") == "AudioVideo;Player;Recorder;");
            }
        }
    }

    GIVEN("An XDG file with entries that is missing values") {
        auto entry = xdg::Entry(singleSectionWithMissingAssignmentLines());

        WHEN("when parsed") {
            entry.parse();

            THEN("it has empty values for missing entries") {
                CHECK(entry.under("Desktop Entry", "Exec"      ) == "");
                CHECK(entry.under("Desktop Entry", "Categories") == "");
            }
        }
    }

    GIVEN("An XDG file with multiple section") {
        auto entry = xdg::Entry(multipleSectionsEntry());

        WHEN("when parsed") {
            entry.parse();

            THEN("values are exposed under the section and key name") {
                CHECK(entry.under("Desktop Entry", "Version"       ) == "1.0");
                CHECK(entry.under("Desktop Entry", "Type"          ) == "Application");
                CHECK(entry.under("Desktop Entry", "Name"          ) == "Xfburn");
                CHECK(entry.under("Desktop Entry", "Comment"       ) == "CD and DVD burning application");
                CHECK(entry.under("Desktop Entry", "Exec"          ) == "xfburn");
                CHECK(entry.under("Desktop Entry", "TryExec"       ) == "xfburn");
                CHECK(entry.under("Desktop Entry", "Icon"          ) == "media-cdrom");
                CHECK(entry.under("Desktop Entry", "MimeType"      ) == "application/x-cd-image;");
                CHECK(entry.under("Desktop Entry", "Terminal"      ) == "false");
                CHECK(entry.under("Desktop Entry", "Categories"    ) == "X-XFCE;GTK;DiscBurning;Archiving;AudioVideo;Utility;");
                CHECK(entry.under("Desktop Entry", "StartupNotify" ) == "true");
                CHECK(entry.under("Desktop Entry", "Actions"       ) == "BurnImage;");

                CHECK(entry.under("Desktop Action BurnImage", "Icon") == "media-cdrom");
                CHECK(entry.under("Desktop Action BurnImage", "Exec") == "xfburn -i %f");
                CHECK(entry.under("Desktop Action BurnImage", "Name") == "Burn Image (xfburn)");
            }

            THEN("it doesn't duplicate keys and values in lower sections") {
                CHECK(entry.under("Desktop Action BurnImage", "TryExec") == "");
            }
        }
    }

    GIVEN("An XDG file without a section") {
        auto entry = xdg::Entry(noDeclarationLines());

        WHEN("when parsed") {
            entry.parse();

            THEN("the values are exposed under an empty section name") {
                CHECK(entry.under("", "Name"      ) == "VLC media player");
                CHECK(entry.under("", "Exec"      ) == "/usr/bin/vlc --started-from-file %U");
                CHECK(entry.under("", "Icon"      ) == "vlc");
                CHECK(entry.under("", "Categories") == "AudioVideo;Player;Recorder;");
            }
        }
    }
}

} // namespace xdg
} // namespace amm
