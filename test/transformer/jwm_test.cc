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

#include "transformer/jwm.h"

#include "../catch.hpp"
#include "representation/menu_start.h"
#include "representation/menu_end.h"
#include "representation/subcategory_start.h"
#include "representation/subcategory_end.h"
#include "representation/program.h"

namespace amm {

SCENARIO("transformer::Jwm", "[transformerjwm]") {
    GIVEN("A JWM transformer") {
        auto transformer = transformer::Jwm();

        WHEN("transforming a menu-start representation") {
            auto menu_start = representation::MenuStart();

            THEN("it is a static message") {
                CHECK(transformer.transform(menu_start) == "<JWM>\n    <!--Menu start-->");
            }
        }

        WHEN("transforming a menu-end representation") {
            auto menu_end = representation::MenuEnd();

            THEN("it is a static message") {
                CHECK(transformer.transform(menu_end) == "    <!--Menu end-->\n</JWM>");
            }
        }

        WHEN("transforming a subcategory-start representation") {
            auto subcategory_start = representation::SubcategoryStart("Application", "application.png");

            THEN("it includes the subcategory name and icon") {
                CHECK(transformer.transform(subcategory_start) == "    <Menu label=\"Application\" icon=\"application.png\">");
            }

            THEN("it XML escapes the name") {
                auto subcategory_start = representation::SubcategoryStart("Fun & Games", "games.png");
                CHECK(transformer.transform(subcategory_start) == "    <Menu label=\"Fun &amp; Games\" icon=\"games.png\">");
            }
        }

        WHEN("transforming a subcategory-end representation") {
            auto subcategory_end = representation::SubcategoryEnd("Application");

            THEN("it includes the subcategory names in XML comments") {
                CHECK(transformer.transform(subcategory_end) == "        <!--Application end-->\n    </Menu>");
            }
        }

        WHEN("transforming a menu-entry representation") {
            auto program = representation::Program("Application", "application.png", "/usr/bin/application", "Application uses");

            THEN("it is a static message") {
                CHECK(transformer.transform(program) == "        <Program label=\"Application\" icon=\"application.png\">/usr/bin/application</Program>");
            }

            THEN("it XML escapes the name") {
                auto program = representation::Program("Shoot & Run", "shooter.png", "/usr/bin/shooter", "First person shooter game");
                CHECK(transformer.transform(program) == "        <Program label=\"Shoot &amp; Run\" icon=\"shooter.png\">/usr/bin/shooter</Program>");
            }

            THEN("it removes field codes from the executable") {
                auto program = representation::Program("Mousepad", "application-text-editor", "mousepad %F", "Simple Text Editor");
                CHECK(transformer.transform(program) == "        <Program label=\"Mousepad\" icon=\"application-text-editor\">mousepad</Program>");
            }
        }
    }
}

} // namespace amm
