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

#include "menu.h"

#include <memory>
#include <string>
#include <vector>

#include "catch.hpp"
#include "representation/menu_start.h"
#include "representation/menu_end.h"
#include "representation/subcategory_start.h"
#include "representation/subcategory_end.h"
#include "representation/program.h"
#include "transformer/transformer_interface.h"
#include "icon_search/icon_search_interface.h"

namespace amm {

static const std::string kapplicationFixturesDirectory = "test/fixtures/applications/";

class TestTransformer : public transformer::TransformerInterface
{
public:
    std::string transform(const representation::MenuStart &entry) const
    {
        return "Menu start--> name: " + entry.name();
    }

    std::string transform(const representation::MenuEnd &entry) const
    {
        return "Menu end--> name: " + entry.name();
    }

    std::string transform(const representation::SubcategoryStart &entry) const
    {
        return "Subsection start--> name: " + entry.name() + " icon: " + entry.icon();
    }

    std::string transform(const representation::SubcategoryEnd &entry) const
    {
        return "Subsection end--> name: " + entry.name();
    }

    std::string transform(const representation::Program &entry) const
    {
        return "Program--> name: " + entry.name() + " icon: " + entry.icon() + " executable: " + entry.executable() + " comment: " + entry.comment();
    }
};

class TestIconSearch : public icon_search::IconSearchInterface
{
public:
    std::string resolvedName(const std::string &name) const { return name + ".always"; }
};


SCENARIO("Menu custom categories", "[menu]") {
    GIVEN("A menu") {
        Menu menu;

        WHEN("loaded with custom categories") {
            auto lines = std::vector<std::string> { "Accessories:accessories:Utility", "Games:games:Game" };
            menu.loadCustomCategories(lines);

            THEN("it stores them in its subcategories") {
                auto subcategories = menu.subcategories();

                REQUIRE(subcategories.size() == 2);
                CHECK(subcategories[0].displayName() == "Accessories");
                CHECK(subcategories[1].displayName() == "Games");
            }
        }

        WHEN("loaded with custom categories with trailing whitespaces") {
            auto lines = std::vector<std::string> { "Accessories:accessories:Utility" };
            menu.loadCustomCategories(lines);

            THEN("it ignores the whitepaces in subcategories") {
                auto subcategories = menu.subcategories();

                REQUIRE(subcategories.size() == 1);
                auto classification_names = subcategories[0].classificationNames();
                REQUIRE(classification_names.size() == 1);
                CHECK(classification_names[0] == "Utility");
            }
        }

        WHEN("given a line that begins with '#'") {
            auto lines = std::vector<std::string> { "# Comments", "#More:comment:here" };
            menu.loadCustomCategories(lines);

            THEN("it ignores the lines beginning with '#'") {
                auto subcategories = menu.subcategories();

                CHECK(subcategories.empty());
            }
        }

        WHEN("given a line with less than three tokens") {
            auto lines = std::vector<std::string> { "Accessories::Accessories", "Game:Games" };
            menu.loadCustomCategories(lines);

            THEN("it ignores the line") {
                auto subcategories = menu.subcategories();

                CHECK(subcategories.empty());
            }
        }


        WHEN("loaded with custom categories with more than three tokens") {
            auto lines = std::vector<std::string> { "Games:games:Game:Fun" };
            menu.loadCustomCategories(lines);

            THEN("its subcategories have multiple classifications") {
                auto subcategories = menu.subcategories();

                REQUIRE(subcategories.size() == 1);
                CHECK(subcategories[0].displayName() == "Games");

                auto classification_names = subcategories[0].classificationNames();
                REQUIRE(classification_names.size() == 2);
                CHECK(classification_names[0] == "Game");
                CHECK(classification_names[1] == "Fun");
            }
        }

        WHEN("given a line with more than three tokens but with some classification names as empty") {
            auto lines = std::vector<std::string> { "Games:games:Game::Fun::Frolic" };
            menu.loadCustomCategories(lines);

            THEN("it ignores the missing classifications") {
                auto subcategories = menu.subcategories();

                REQUIRE(subcategories.size() == 1);

                auto classification_names = subcategories[0].classificationNames();
                REQUIRE(classification_names.size() == 3);
                CHECK(classification_names[0] == "Game");
                CHECK(classification_names[1] == "Fun");
                CHECK(classification_names[2] == "Frolic");
            }
        }

        WHEN("given a line without a single classification name") {
            auto lines = std::vector<std::string>  { "Games:games:::" };
            menu.loadCustomCategories(lines);

            THEN("it ignores the line with missing classification names") {
                auto subcategories = menu.subcategories();

                CHECK(subcategories.empty());
            }
        }
    }
}

SCENARIO("Menu statistics", "[menu]") {
    GIVEN("A menu") {
        Menu menu;

        WHEN("successfully parsed") {
            auto files = std::vector<std::string> { kapplicationFixturesDirectory + "vlc.desktop", kapplicationFixturesDirectory + "mousepad.desktop" };
            menu.populate(files);
            Stats summary = menu.summary();

            THEN("it has number of total files") {
                CHECK(summary.totalParsedFiles() == 2);
            }
        }

        WHEN("parsing an unclassified desktop file") {
            auto files = std::vector<std::string> { kapplicationFixturesDirectory + "unclassified.desktop", kapplicationFixturesDirectory + "mousepad.desktop" };
            menu.populate(files);
            Stats summary = menu.summary();

            THEN("it has the number of unclassified files") {
                CHECK(summary.totalUnclassifiedFiles() == 1);
            }

            THEN("it has the categories that weren't classified") {
                std::vector<std::string> unhandled_classifications = summary.unhandledClassifications();

                REQUIRE(unhandled_classifications.size() == 1);
                CHECK(unhandled_classifications[0] == "GTK");
            }
        }

        WHEN("parsing a file suppressed with 'NoDisplay=true'") {
            auto files = std::vector<std::string> { kapplicationFixturesDirectory + "mousepad.desktop", kapplicationFixturesDirectory + "suppressed.desktop" };
            menu.populate(files);
            Stats summary = menu.summary();

            THEN("it has the number of suppressed files") {
                CHECK(summary.totalSuppressedFiles() == 1);
            }
        }

        WHEN("parsing a file with missing entries") {
            auto files = std::vector<std::string> { kapplicationFixturesDirectory + "vlc.desktop", kapplicationFixturesDirectory + "missing.desktop" };
            menu.populate(files);
            Stats summary = menu.summary();

            THEN("it has a list of the unparsed files") {
                std::vector<std::string> unparsed_files = summary.unparsedFiles();

                REQUIRE(unparsed_files.size() == 1);
                CHECK(unparsed_files[0] == kapplicationFixturesDirectory + "missing.desktop");
            }
        }

        WHEN("parsing a file with missing and suppressed entries") {
            auto files = std::vector<std::string> {
                kapplicationFixturesDirectory + "suppressed.desktop",
                kapplicationFixturesDirectory + "suppressedinvalid.desktop",
                kapplicationFixturesDirectory + "missing.desktop",
            };
            menu.populate(files);
            Stats summary = menu.summary();

            THEN("the missing entries don't include suppressed files") {
                std::vector<std::string> unparsed_files = summary.unparsedFiles();

                REQUIRE(unparsed_files.size() == 1);
                CHECK(unparsed_files[0] == kapplicationFixturesDirectory + "missing.desktop");
            }
        }
    }
}

SCENARIO("Menu sort", "[menu]") {
    GIVEN("A menu") {
        auto lines = std::vector<std::string> { "Multimedia:multimedia:AudioVideo", "Utilities:utilities:Utility" };

        auto files = std::vector<std::string> {
            kapplicationFixturesDirectory + "vlc.desktop",
            kapplicationFixturesDirectory + "nested/xfburn.desktop",
            kapplicationFixturesDirectory + "nested/deepnested/whaawmp.desktop",
            kapplicationFixturesDirectory + "mousepad.desktop",
        };

        Menu menu;
        menu.loadCustomCategories(lines);
        menu.populate(files);

        WHEN("sorted") {
            menu.sort();

            THEN("the individual entries inside subcategories are sorted alphabetically") {
                auto subcategories = menu.subcategories();
                REQUIRE(subcategories.size() == 3);

                CHECK(subcategories[0].displayName() == "Multimedia");
                auto multimedia_files = subcategories[0].desktopEntries();
                REQUIRE(multimedia_files.size() == 3);
                CHECK(multimedia_files[0].name() == "VLC media player");
                CHECK(multimedia_files[1].name() == "Whaaw! Media Player");
                CHECK(multimedia_files[2].name() == "Xfburn");

                CHECK(subcategories[1].displayName() == "Utilities");
                auto utility_files = subcategories[1].desktopEntries();
                REQUIRE(utility_files.size() == 2);
                CHECK(utility_files[0].name() == "Mousepad");
                CHECK(utility_files[1].name() == "Xfburn");


                REQUIRE(subcategories[2].displayName() == "Others");
                auto other_files = subcategories[2].desktopEntries();
                CHECK(other_files.empty());
            }
        }
    }
}

SCENARIO("Menu representations", "[menu]") {
    GIVEN("A menu") {
        Menu menu;

        WHEN("transformed to representations") {
            auto files = std::vector<std::string> { kapplicationFixturesDirectory + "vlc.desktop", kapplicationFixturesDirectory + "mousepad.desktop" };

            menu.populate(files);
            auto representations = menu.representations();
            TestTransformer test_transformer;

            THEN("it stores menu start, subcategory start, menu entry, subcategory end and menu end") {
                REQUIRE(representations.size() == 8);

                CHECK(representations[0]->visit(test_transformer) == "Menu start--> name: Menu start");
                CHECK(representations[1]->visit(test_transformer) == "Subsection start--> name: Accessories icon: applications-accessories");
                CHECK(representations[2]->visit(test_transformer) == "Program--> name: Mousepad icon: accessories-text-editor executable: mousepad %F comment: Simple Text Editor");
                CHECK(representations[3]->visit(test_transformer) == "Subsection end--> name: Accessories end");
                CHECK(representations[4]->visit(test_transformer) == "Subsection start--> name: Multimedia icon: applications-multimedia");
                CHECK(representations[5]->visit(test_transformer) == "Program--> name: VLC media player icon: vlc executable: /usr/bin/vlc --started-from-file %U comment: Read, capture, broadcast your multimedia streams");
                CHECK(representations[6]->visit(test_transformer) == "Subsection end--> name: Multimedia end");
                CHECK(representations[7]->visit(test_transformer) == "Menu end--> name: Menu end");
            }
        }

        WHEN("transformed to a representations with icon service") {
            auto files = std::vector<std::string> { kapplicationFixturesDirectory + "vlc.desktop", kapplicationFixturesDirectory + "mousepad.desktop" };

            auto *icon_searcher = new TestIconSearch;
            menu.registerIconService(new TestIconSearch);

            menu.populate(files);
            auto representations = menu.representations();
            TestTransformer test_transformer;

            THEN("it adds the icon name to icons for subcategory and menu-entry") {
                REQUIRE(representations.size() == 8);

                CHECK(representations[0]->visit(test_transformer) == "Menu start--> name: Menu start");
                CHECK(representations[1]->visit(test_transformer) == "Subsection start--> name: Accessories icon: applications-accessories.always");
                CHECK(representations[2]->visit(test_transformer) == "Program--> name: Mousepad icon: accessories-text-editor.always executable: mousepad %F comment: Simple Text Editor");
                CHECK(representations[3]->visit(test_transformer) == "Subsection end--> name: Accessories end");
                CHECK(representations[4]->visit(test_transformer) == "Subsection start--> name: Multimedia icon: applications-multimedia.always");
                CHECK(representations[5]->visit(test_transformer) == "Program--> name: VLC media player icon: vlc.always executable: /usr/bin/vlc --started-from-file %U comment: Read, capture, broadcast your multimedia streams");
                CHECK(representations[6]->visit(test_transformer) == "Subsection end--> name: Multimedia end");
                CHECK(representations[7]->visit(test_transformer) == "Menu end--> name: Menu end");
            }
        }

        WHEN("transformed to a representations with a custom language") {
            auto files = std::vector<std::string> { kapplicationFixturesDirectory + "vlc.desktop", kapplicationFixturesDirectory + "mousepad.desktop" };

            menu.registerLanguage("sr");

            menu.populate(files);
            auto representations = menu.representations();
            TestTransformer test_transformer;

            THEN("it uses names in the given language when available") {
                REQUIRE(representations.size() == 8);

                CHECK(representations[0]->visit(test_transformer) == "Menu start--> name: Menu start");
                CHECK(representations[1]->visit(test_transformer) == "Subsection start--> name: Accessories icon: applications-accessories");
                CHECK(representations[2]->visit(test_transformer) == "Program--> name: Мишоловка icon: accessories-text-editor executable: mousepad %F comment: Једноставан уређивач текста");
                CHECK(representations[3]->visit(test_transformer) == "Subsection end--> name: Accessories end");
                CHECK(representations[4]->visit(test_transformer) == "Subsection start--> name: Multimedia icon: applications-multimedia");
                CHECK(representations[5]->visit(test_transformer) == "Program--> name: VLC media player icon: vlc executable: /usr/bin/vlc --started-from-file %U comment: Read, capture, broadcast your multimedia streams");
                CHECK(representations[6]->visit(test_transformer) == "Subsection end--> name: Multimedia end");
                CHECK(representations[7]->visit(test_transformer) == "Menu end--> name: Menu end");
            }
        }
    }
}

} // namespace amm
