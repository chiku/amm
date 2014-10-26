/*
  This file is part of mjwm.
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

#include "stats.h"

#include <string>
#include <vector>

#include "catch.hpp"

namespace amm {

static Stats populatedStats() {
    Stats stats;
    stats.addClassifiedFile("VLC");
    stats.addClassifiedFile("Mousepad");
    stats.addClassifiedFile("Firefox");
    stats.addUnclassifiedFile("htop");
    stats.addUnclassifiedFile("NEdit");
    stats.addUnparsedFile("daemon");
    stats.addSuppressedFile("mplayer");
    return stats;
}

static std::vector<std::string> unhandledClassificationFirstSet() {
    std::vector<std::string> classifications;
    classifications.push_back("Calculator");
    classifications.push_back("Player");
    classifications.push_back("Archiving");
    return classifications;
}

static std::vector<std::string> unhandledClassificationSecondSet() {
    std::vector<std::string> classifications;
    classifications.push_back("Browser");
    classifications.push_back("Player");
    classifications.push_back("WordProcessor");
    classifications.push_back("Archiving");
    return classifications;
}

static std::string expectedEmptydetails() {
    return "Total desktop files: 0\n"
                 "Parsed desktop files: 0\n"
                 "Unparsed desktop files: 0\n"
                 "Suppressed desktop files (NoDisplay): 0\n"
                 "Unclassified desktop files: 0\n";
}

static std::string expectedShortdetailsWithValues() {
    return "Total desktop files: 7\n"
                 "Parsed desktop files: 5\n"
                 "Unparsed desktop files: 1\n"
                 "Suppressed desktop files (NoDisplay): 1\n"
                 "Unclassified desktop files: 2\n";
}

SCENARIO("Stats totals", "[stats]") {
    GIVEN("A stat") {
        Stats stats;

        WHEN("created") {
            THEN("it has no total files")        { REQUIRE(stats.totalFiles() == 0); }
            THEN("it has no parsed files")       { REQUIRE(stats.totalParsedFiles() == 0); }
            THEN("it has no unclassified files") { REQUIRE(stats.totalUnclassifiedFiles() == 0); }
            THEN("it has no suppressed files")   { REQUIRE(stats.totalSuppressedFiles() == 0); }
            THEN("it has no unparsed files")     { REQUIRE(stats.totalUnparsedFiles() == 0); }

            THEN("is has no counts in short details") {
                REQUIRE(stats.details("short") == expectedEmptydetails());
            }

            THEN("is has no counts in details") {
                REQUIRE(stats.details("normal") == expectedEmptydetails());
            }

            THEN("is has no counts in long details") {
                REQUIRE(stats.details("long") == expectedEmptydetails());
            }
        }

        WHEN("a classified file is added") {
            stats.addClassifiedFile("file");

            THEN("it has one total file")        { REQUIRE(stats.totalFiles() == 1); }
            THEN("it has one parsed file")       { REQUIRE(stats.totalParsedFiles() == 1); }
            THEN("it has no unclassified files") { REQUIRE(stats.totalUnclassifiedFiles() == 0); }
            THEN("it has no suppressed files")   { REQUIRE(stats.totalSuppressedFiles() == 0); }
            THEN("it has no unparsed files")     { REQUIRE(stats.totalUnparsedFiles() == 0); }
        }

        WHEN("an unclassified file is added") {
            stats.addUnclassifiedFile("file");

            THEN("it has one total file")        { REQUIRE(stats.totalFiles() == 1); }
            THEN("it has one parsed file")       { REQUIRE(stats.totalParsedFiles() == 1); }
            THEN("it has one unclassified file") { REQUIRE(stats.totalUnclassifiedFiles() == 1); }
            THEN("it has no suppressed files")   { REQUIRE(stats.totalSuppressedFiles() == 0); }
            THEN("it has no unparsed files")     { REQUIRE(stats.totalUnparsedFiles() == 0); }
        }

        WHEN("a suppressed file is added") {
            stats.addSuppressedFile("file");

            THEN("it has one total file")        { REQUIRE(stats.totalFiles() == 1); }
            THEN("it has no parsed files")       { REQUIRE(stats.totalParsedFiles() == 0); }
            THEN("it has no unclassified files") { REQUIRE(stats.totalUnclassifiedFiles() == 0); }
            THEN("it has one suppressed file")   { REQUIRE(stats.totalSuppressedFiles() == 1); }
            THEN("it has no unparsed files")     { REQUIRE(stats.totalUnparsedFiles() == 0); }
        }

        WHEN("an unparsed file is added") {
            stats.addUnparsedFile("file");

            THEN("it has one total file")        { REQUIRE(stats.totalFiles() == 1); }
            THEN("it has no parsed files")       { REQUIRE(stats.totalParsedFiles() == 0); }
            THEN("it has no unclassified files") { REQUIRE(stats.totalUnclassifiedFiles() == 0); }
            THEN("it has no suppressed files")   { REQUIRE(stats.totalSuppressedFiles() == 0); }
            THEN("it has one unparsed files")    { REQUIRE(stats.totalUnparsedFiles() == 1); }
        }
    }
}


SCENARIO("Stats summaries", "[stats]") {
    GIVEN("A stat") {
        Stats stats;

        WHEN("different types of files are added") {
            Stats stats = populatedStats();

            THEN("short details includes counts") {
                REQUIRE(stats.details("short") == expectedShortdetailsWithValues());
            }

            THEN("details includes short details and a list of unparsed files") {
                std::string expected_details = expectedShortdetailsWithValues() + "List of unparsed files: daemon\n";
                REQUIRE(stats.details("normal") == expected_details);
            }

            THEN("long details includes details and lists of suppressed and unclassified files") {
                std::string expected_details = expectedShortdetailsWithValues() +
                                                "List of unparsed files: daemon\n"
                                                "List of suppressed files: mplayer\n"
                                                "List of unclassified files: htop, NEdit\n";
                REQUIRE(stats.details("long") == expected_details);
            }
        }

        WHEN("unhandled classifications are added") {
            Stats stats = populatedStats();
            stats.addUnhandledClassifications(unhandledClassificationFirstSet());
            stats.addUnhandledClassifications(unhandledClassificationSecondSet());

            THEN("long details includes the unhandled classifications") {
                std::string expected_details = expectedShortdetailsWithValues() +
                                                "List of unparsed files: daemon\n"
                                                "List of suppressed files: mplayer\n"
                                                "List of unclassified files: htop, NEdit\n"
                                                "List of unhandled classifications: Archiving, Browser, Calculator, Player, WordProcessor\n";
                REQUIRE(stats.details("long") == expected_details);
            }
        }
    }
}

} // namespace amm
