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

#include "filex.h"

#include <cstdio>
#include <string>
#include <vector>

#include "catch.hpp"

namespace amm {

SCENARIO("FileX", "[filex]") {
    GIVEN("A filex") {
        WHEN("pointing to a file that exists") {
            auto filex = FileX("test/fixtures/applications/vlc.desktop");

            THEN("it exists") {
                CHECK(filex.exists());
            }

            THEN("it isn't a directory") {
                CHECK_FALSE(filex.existsAsDirectory());
            }

            THEN("it succeeds in reading its contents") {
                auto lines = std::vector<std::string> {};
                CHECK(filex.readLines(&lines));
            }

            THEN("it reads the content of the file") {
                auto lines = std::vector<std::string> {};
                filex.readLines(&lines);
                CHECK(lines == (std::vector<std::string> {
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
                }));
            }

            THEN("it over-writes the older content") {
                auto lines = std::vector<std::string> {};
                filex.readLines(&lines);
                filex.readLines(&lines);
                CHECK(lines.size() == 11);
            }

            THEN("it doesn't write lines to the file") {
                auto lines = std::vector<std::string> { "first", "second" };
                CHECK_FALSE(filex.writeLines(lines));
            }

            THEN("it can move the file to an existing directory") {
                auto file_name = std::string { "test/fixtures/new-file" };
                auto renamed_file_name = std::string { "test/fixtures/renamed-file" };

                remove(file_name.c_str());
                remove(renamed_file_name.c_str());

                auto lines = std::vector<std::string> { "first", "second" };
                CHECK(FileX(file_name).writeLines(lines));

                CHECK(FileX(file_name).moveTo(renamed_file_name));

                CHECK_FALSE(FileX(file_name).exists());

                auto read_lines = std::vector<std::string> {};
                CHECK(FileX(renamed_file_name).readLines(&read_lines));
                CHECK(read_lines == (std::vector<std::string> { "first", "second" }));

                remove(file_name.c_str());
                remove(renamed_file_name.c_str());
            }

            THEN("it can't move the file to a non-existing directory") {
                auto file_name = std::string { "test/fixtures/new-file" };
                auto renamed_file_name = std::string { "test/does-not-exist-fixtures/renamed-file"};

                remove(file_name.c_str());
                remove(renamed_file_name.c_str());

                auto lines = std::vector<std::string> { "first", "second" };
                CHECK(FileX(file_name).writeLines(lines));

                CHECK_FALSE(FileX(file_name).moveTo(renamed_file_name));

                CHECK(FileX(file_name).exists());
                CHECK_FALSE(FileX(renamed_file_name).exists());

                remove(file_name.c_str());
                remove(renamed_file_name.c_str());
            }
        }

        WHEN("pointing to a directory that exists") {
            FileX dirx("test/fixtures/applications");

            THEN("it exists") {
                CHECK(dirx.exists());
            }

            THEN("it is a directory") {
                CHECK(dirx.existsAsDirectory());
            }
        }

        WHEN("pointing to a file that doesn't exist") {
            auto file_name = std::string { "test/fixtures/applications/does-not-exist.desktop" };
            auto filex = FileX(file_name);
            remove(file_name.c_str());
            remove((file_name + ".backup_extension").c_str());

            THEN("it doesn't exist") {
                CHECK_FALSE(filex.exists());
            }

            THEN("it isn't a directory") {
                CHECK_FALSE(filex.existsAsDirectory());
            }

            THEN("it fails to read its contents") {
                auto lines = std::vector<std::string> {};
                CHECK_FALSE(filex.readLines(&lines));
            }

            THEN("its lines are empty") {
                auto lines = std::vector<std::string> {};
                filex.readLines(&lines);
                CHECK(lines.empty());
            }

            THEN("it doesn't over-write the older contents") {
                auto lines = std::vector<std::string> { "existing line" };
                filex.readLines(&lines);
                CHECK(lines == (std::vector<std::string> { "existing line" } ));
            }

            THEN("it writes lines to the file") {
                auto lines = std::vector<std::string> { "first", "second" };
                CHECK(filex.writeLines(lines));

                auto read_file = FileX(file_name);
                auto read_lines = std::vector<std::string> {};
                CHECK(read_file.readLines(&read_lines));
                CHECK(lines == (std::vector<std::string> { "first", "second" } ));
                remove(file_name.c_str());
            }
        }
    }
}

} // namespace amm
