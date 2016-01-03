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

#include "xdg/icon_theme.h"

#include <vector>
#include <string>

#include "../catch.hpp"

namespace amm {
namespace xdg {

std::vector<std::string> hicolorThemeLines()
{
    return std::vector<std::string> {
        "[Icon Theme]",
        "Name=Hicolor",
        "Directories=48x48/apps,48x48/mimetypes,32x32/apps,scalable/apps,scalable/mimetypes",
    };
}

std::vector<std::string> birchIconThemeLinesWithoutParent()
{
    return std::vector<std::string> {
        "[Icon Theme]",
        "Name=Birch",
        "Directories=48x48/apps,48x48/mimetypes,32x32/apps,scalable/apps,scalable/mimetypes",
        "Name[sv]=Björk",
        "Comment=Icon theme with a wooden look",
        "Comment[sv]=Träinspirerat ikontema",
        "Directories=48x48/apps,48x48/mimetypes,32x32/apps,scalable/apps,scalable/mimetypes",
    };
}

std::vector<std::string> birchIconThemeLines()
{
    auto lines = birchIconThemeLinesWithoutParent();
    lines.push_back("Inherits=wood,default");
    return lines;
}

std::vector<std::string> subdirectoryLinesForScalableApps()
{
    return std::vector<std::string> {
        "[scalable/apps]",
        "Size=48",
        "Type=Scalable",
        "MinSize=1",
        "MaxSize=256",
        "Threshold=208",
        "Context=Applications",
        "",
    };
}

std::vector<std::string> joinLists(std::vector<std::string> first, std::vector<std::string> second)
{
    first.insert(first.end(), second.begin(), second.end());
    return first;
}

SCENARIO("xdg::IconTheme", "[icontheme]") {
    GIVEN("An Icon Theme") {
        WHEN("created") {
            auto icon_theme = IconTheme(birchIconThemeLines());

            THEN("it has a name") {
                CHECK(icon_theme.name() == "Birch");
            }

            THEN("it has a list of parents") {
                CHECK(icon_theme.parents() == (std::vector<std::string> { "wood", "default" }));
            }

            THEN("it has a list of directories") {
                auto directories = icon_theme.directories();
                REQUIRE(directories.size() == 5);
                CHECK(directories[0].name() == "48x48/apps");
                CHECK(directories[1].name() == "48x48/mimetypes");
                CHECK(directories[2].name() == "32x32/apps");
                CHECK(directories[3].name() == "scalable/apps");
                CHECK(directories[4].name() == "scalable/mimetypes");
            }
        }

        WHEN("registered with its internal name") {
            auto icon_theme = IconTheme(birchIconThemeLines());
            icon_theme.internalNameIs("birch");

            THEN("it can be referred to by its internal name") {
                CHECK(icon_theme.isNamed("birch"));
            }

            THEN("it can be referred to by its display name") {
                CHECK(icon_theme.isNamed("Birch"));
            }

            THEN("it can't be referred to other names") {
                CHECK(!icon_theme.isNamed("Wood"));
            }
        }

        WHEN("without explicit parents") {
            auto icon_theme = IconTheme(birchIconThemeLinesWithoutParent());

            THEN("its parent is Hicolor") {
                CHECK(icon_theme.parents() == (std::vector<std::string> { "Hicolor" }));
            }
        }

        WHEN("Hicolor") {
            auto icon_theme = IconTheme(hicolorThemeLines());

            THEN("it has no parents") {
                CHECK(icon_theme.parents().empty());
            }
        }
    }

    GIVEN("An Icon Theme with sub-directories") {
        auto icon_theme = IconTheme(joinLists(birchIconThemeLines(), subdirectoryLinesForScalableApps()));
        auto directories = icon_theme.directories();

        WHEN("the sub-directory has all properties") {
            auto scalable_apps = directories[3];

            THEN("the sub-directory has a size") {
                CHECK(scalable_apps.size() == 48);
            }

            THEN("the sub-directory has a type") {
                CHECK(scalable_apps.type() == SCALABLE);
            }

            THEN("the sub-directory has a maximum size") {
                CHECK(scalable_apps.maxSize() == 256);
            }

            THEN("the sub-directory has a threshold") {
                CHECK(scalable_apps.threshold() == 208);
            }
        }
    }
}

} // namespace xdg
} // namespace amm
