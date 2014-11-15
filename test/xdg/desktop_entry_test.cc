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

#include "xdg/desktop_entry.h"

#include <vector>
#include <string>

#include "../catch.hpp"

namespace amm {
namespace xdg {

SCENARIO("Populate DesktopEntry", "[desktopfile]") {
    GIVEN("A desktop-file") {
        DesktopEntry entry;

        WHEN("created") {
            auto lines = std::vector<std::string> {
                "[Desktop Entry]\n",
                "Name=Mousepad\n",
                "Icon=accessories-text-editor\n",
                "Exec=mousepad %F\n",
                "Categories=Application;Utility;TextEditor;GTK;\n",
                "Comment=Simple Text Editor\n",
            };
            entry.parse(lines);

            THEN("it has a name") {
                CHECK(entry.name() == "Mousepad");
            }

            THEN("it has an icon") {
                CHECK(entry.icon() == "accessories-text-editor");
            }

            THEN("it has an executable") {
                CHECK(entry.executable() == "mousepad %F");
            }

            THEN("it has categories") {
                auto categories = entry.categories();
                REQUIRE(categories.size() == 4);
                CHECK(categories[0] == "Application");
                CHECK(categories[1] == "GTK");
                CHECK(categories[2] == "TextEditor");
                CHECK(categories[3] == "Utility");
            }

            THEN("it has a comment") {
                CHECK(entry.comment() == "Simple Text Editor");
            }

            THEN("it knows if it would be displayed by default") {
                CHECK(entry.display());
            }

            WHEN("NoDisplay is set to true") {
                auto lines = std::vector<std::string> {
                    "[Desktop Entry]\n",
                    "NoDisplay=true\n",
                };
                entry.parse(lines);
                THEN("it is not marked as displayed") {
                    CHECK_FALSE(entry.display());
                }
            }

            WHEN("NoDisplay is set to 1") {
                auto lines = std::vector<std::string> {
                    "[Desktop Entry]\n",
                    "NoDisplay=1\n",
                };
                entry.parse(lines);
                THEN("it is not marked as displayed") {
                    CHECK_FALSE(entry.display());
                }
            }
        }
    }
}

SCENARIO("Language-aware DesktopEntry", "[focus]") {
    GIVEN("A desktop-file") {
        DesktopEntry entry;
        entry.hasLanguage("sr");

        WHEN("created") {
            auto lines = std::vector<std::string> {
                "[Desktop Entry]\n",
                "Name=Mousepad\n",
                "Name[sr]=Мишоловка\n",
                "Icon=accessories-text-editor\n",
                "Exec=mousepad %F\n",
                "Categories=Application;Utility;TextEditor;GTK;\n",
                "Comment=Simple Text Editor\n",
                "Comment[sr]=Једноставан уређивач текста\n",
            };
            entry.parse(lines);

            THEN("it picks the name with matching language") {
                CHECK(entry.name() == "Мишоловка");
            }

            THEN("it picks comment with the matching language") {
                CHECK(entry.comment() == "Једноставан уређивач текста");
            }
        }
    }
}


SCENARIO("DesktopEntry comparisons", "[desktopfile]") {
    DesktopEntry entry;
    DesktopEntry other_entry;

    GIVEN("A desktop-file") {
        auto lines = std::vector<std::string> {
            "[Desktop Entry]\n",
            "Name=Mousepad",
            "Exec=mousepad",
        };
        entry.parse(lines);

        WHEN("compared to another desktop-file") {
            WHEN("the other desktop-file has an alphabetically greater name") {
                auto other_lines = std::vector<std::string> {
                    "[Desktop Entry]\n",
                    "Name=VLC",
                };
                other_entry.parse(other_lines);

                THEN("the desktop file is lesser than the other desktop file") {
                    CHECK(entry < other_entry);
                    CHECK_FALSE(other_entry < entry);
                }
            }

            WHEN("the other desktop-file has an alphabetically lesser name") {
                auto other_lines = std::vector<std::string> {
                    "[Desktop Entry]\n",
                    "Name=GParted",
                };
                other_entry.parse(other_lines);

                THEN("the desktop file is greater than the other desktop file") {
                    CHECK(entry > other_entry);
                    CHECK_FALSE(other_entry > entry);
                }
            }

            WHEN("the other desktop-file has same name and executable") {
                auto other_lines = std::vector<std::string> {
                    "[Desktop Entry]\n",
                    "Name=Mousepad",
                    "Exec=mousepad",
                };
                other_entry.parse(other_lines);

                THEN("the desktop file is equal to the other desktop file") {
                    CHECK(entry == other_entry);
                    CHECK_FALSE(other_entry != entry);
                }
            }

            WHEN("the other desktop-file has a different name") {
                auto other_lines = std::vector<std::string> {
                    "[Desktop Entry]\n",
                    "Name=VLC Server",
                    "Exec=vlc",
                };
                other_entry.parse(other_lines);

                THEN("the desktop file is not equal to the other desktop file") {
                    CHECK(entry != other_entry);
                    CHECK_FALSE(other_entry == entry);
                }
            }

            WHEN("the other desktop-file has a different executable") {
                auto other_lines = std::vector<std::string> {
                    "[Desktop Entry]\n",
                    "Name=VLC",
                    "Exec=svlc",
                };
                other_entry.parse(other_lines);

                THEN("the desktop file is not equal to the other desktop file") {
                    CHECK(entry != other_entry);
                    CHECK_FALSE(other_entry == entry);
                }
            }
        }
    }
}


SCENARIO("DesktopEntry validity", "[desktopfile]") {
    DesktopEntry entry;

    GIVEN("A desktop-file") {
        WHEN("it has a name, an icon and an executable") {
            auto lines = std::vector<std::string> {
                "[Desktop Entry]\n",
                "Name=Mousepad\n",
                "Icon=accessories-text-editor\n",
                "Exec=mousepad %F\n",
            };
            entry.parse(lines);
            THEN("it is valid") {
                CHECK(entry.isValid());
            }
        }

        WHEN("it has a no name") {
            auto lines = std::vector<std::string> {
                "[Desktop Entry]\n",
                "Icon=accessories-text-editor\n",
                "Exec=mousepad %F\n",
            };
            entry.parse(lines);
            THEN("it is not valid") {
                CHECK_FALSE(entry.isValid());
            }
        }

        WHEN("it has no icon") {
            auto lines = std::vector<std::string> {
                "[Desktop Entry]\n",
                "Name=Mousepad\n",
                "Exec=mousepad %F\n",
            };
            entry.parse(lines);
            THEN("it is not valid") {
                CHECK_FALSE(entry.isValid());
            }
        }

        WHEN("it has no executable") {
            auto lines = std::vector<std::string> {
                "[Desktop Entry]\n",
                "Name=Mousepad\n",
                "Icon=accessories-text-editor\n",
            };
            entry.parse(lines);
            THEN("it is not valid") {
                CHECK_FALSE(entry.isValid());
            }
        }
    }
}

SCENARIO("DesktopEntry classifications", "[desktopfile]") {
    DesktopEntry entry;

    GIVEN("A desktop-file") {
        WHEN("it has one of the categories as AudioVideo") {
            auto lines = std::vector<std::string> {
                "[Desktop Entry]\n",
                "Categories=AudioVideo;Audio;Player;GTK;\n",
            };
            entry.parse(lines);
            THEN("it is an AudioVideo") {
                CHECK(entry.isA("AudioVideo"));
            }

            THEN("it is either of AudioVideo and Multimedia") {
                auto classifications = std::vector<std::string> { "AudioVideo", "Multimedia" };
                CHECK(entry.isAnyOf(classifications));
            }
        }

        WHEN("it has none of the categories as AudioVideo") {
            auto lines = std::vector<std::string> {
                "[Desktop Entry]\n",
                "Categories=Audio;Video;Player;GTK;\n",
            };
            entry.parse(lines);
            THEN("it is not an AudioVideo") {
                CHECK_FALSE(entry.isA("AudioVideo"));
            }

            THEN("it is not either of AudioVideo or Multimedia") {
                auto classifications = std::vector<std::string> { "AudioVideo", "Multimedia" };
                CHECK_FALSE(entry.isAnyOf(classifications));
            }
        }
    }
}

} // namespace xdg
} // namespace amm
