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

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdio>

#include "../QUnit.hpp"

#include "../../src/icon_service.h"
#include "../../src/jwm/menu.h"

namespace amm
{
	const std::string fixtures_directory = "test/fixtures/";

	namespace jwm
	{
		// Verifies convertion of a list of FreeDesktop .desktop files to a JWM menu
		class menu_test
		{
			QUnit::UnitTest qunit;
			amm::icon_service icon_service;
			amm::icon_service icon_service_with_png;

			void setup()
			{
				icon_service_with_png.register_extension(".png");
			}

			void test_jwm_menu_counts_total_desktop_files_parsed_successfully()
			{
				std::vector<std::string> files;
				files.push_back(fixtures_directory + "vlc.desktop");
				files.push_back(fixtures_directory + "mousepad.desktop");
				menu menu(files, icon_service);

				menu.populate();

				QUNIT_IS_EQUAL(2, menu.total_parsed_files());
			}

			void test_jwm_menu_counts_total_unclassified_desktop_files_parsed_successfully()
			{
				std::vector<std::string> files;
				files.push_back(fixtures_directory + "unclassified.desktop");
				files.push_back(fixtures_directory + "mousepad.desktop");
				menu menu(files, icon_service);

				menu.populate();

				QUNIT_IS_EQUAL(1, menu.total_unclassified_parsed_files());
			}

			void test_jwm_menu_has_a_list_of_unparsed_files()
			{
				std::vector<std::string> files;
				files.push_back(fixtures_directory + "vlc.desktop");
				files.push_back(fixtures_directory + "missing.desktop");
				menu menu(files, icon_service);

				menu.populate();

				std::vector<std::string> unparsed_files = menu.unparsed_file_names();
				QUNIT_IS_EQUAL(1, unparsed_files.size());
				QUNIT_IS_EQUAL(fixtures_directory + "missing.desktop", unparsed_files[0]);
			}

			void test_jwm_menu_is_serilizable_to_output_stream()
			{
				std::vector<std::string> files;
				files.push_back(fixtures_directory + "vlc.desktop");
				files.push_back(fixtures_directory + "mousepad.desktop");
				menu menu(files, icon_service);

				menu.populate();
				menu.sort();
				std::stringstream stream;
				stream << menu;

				std::string expected_content = "\
<JWM>\n\
  <Menu label=\"Accessories\" icon=\"accessories\">\n\
    <Program label=\"Mousepad\" icon=\"accessories-text-editor\">mousepad</Program>\n\
  </Menu>\n\
  <Menu label=\"Multimedia\" icon=\"multimedia\">\n\
    <Program label=\"VLC media player\" icon=\"vlc\">/usr/bin/vlc --started-from-file</Program>\n\
  </Menu>\n\
</JWM>\n\
";
				QUNIT_IS_EQUAL(expected_content, stream.str());
			}

			void test_jwm_menu_serialization_includes_icon_extension_when_present()
			{
				std::vector<std::string> files;
				files.push_back(fixtures_directory + "vlc.desktop");
				files.push_back(fixtures_directory + "mousepad.desktop");
				menu menu(files, icon_service_with_png);

				menu.populate();
				menu.sort();
				std::stringstream stream;
				stream << menu;

				std::string expected_content = "\
<JWM>\n\
  <Menu label=\"Accessories\" icon=\"accessories.png\">\n\
    <Program label=\"Mousepad\" icon=\"accessories-text-editor.png\">mousepad</Program>\n\
  </Menu>\n\
  <Menu label=\"Multimedia\" icon=\"multimedia.png\">\n\
    <Program label=\"VLC media player\" icon=\"vlc.png\">/usr/bin/vlc --started-from-file</Program>\n\
  </Menu>\n\
</JWM>\n\
";
				QUNIT_IS_EQUAL(expected_content, stream.str());
			}

			void test_jwm_menu_serialization_has_unclassified_entries_at_the_end()
			{
				std::vector<std::string> files;
				files.push_back(fixtures_directory + "unclassified.desktop");
				files.push_back(fixtures_directory + "mousepad.desktop");
				menu menu(files, icon_service);

				menu.populate();
				menu.sort();
				std::stringstream stream;
				stream << menu;

				std::string expected_content = "\
<JWM>\n\
  <Menu label=\"Accessories\" icon=\"accessories\">\n\
    <Program label=\"Mousepad\" icon=\"accessories-text-editor\">mousepad</Program>\n\
  </Menu>\n\
  <Menu label=\"Others\" icon=\"others\">\n\
    <Program label=\"Unclassified\" icon=\"unclassified\">unclassify</Program>\n\
  </Menu>\n\
</JWM>\n\
";
				QUNIT_IS_EQUAL(expected_content, stream.str());
			}

			void test_jwm_menu_serialization_skips_files_with_missing_content()
			{
				std::vector<std::string> files;
				files.push_back(fixtures_directory + "missing.desktop");
				menu menu(files, icon_service);

				menu.populate();
				menu.sort();
				std::stringstream stream;
				stream << menu;

				QUNIT_IS_EQUAL("<JWM>\n</JWM>\n", stream.str());
			}

			bool assert_start_with(std::string sentence, std::string fragment)
			{
				if (std::mismatch(fragment.begin(), fragment.end(), sentence.begin()).first == fragment.end()) {
					return true;
				} else {
					std::cout << "Sentence: >>" << sentence << "<<" << std::endl
					          << "Doesn't start with: >>" << fragment << "<<" << std::endl;
					return false;
				}
				return std::mismatch(fragment.begin(), fragment.end(), sentence.begin()).first == fragment.end();
			}

		public:
			menu_test(std::ostream &out, int verbose_level) : qunit(out, verbose_level) {}

			int run()
			{
				setup();
				test_jwm_menu_counts_total_desktop_files_parsed_successfully();
				test_jwm_menu_counts_total_unclassified_desktop_files_parsed_successfully();
				test_jwm_menu_has_a_list_of_unparsed_files();
				test_jwm_menu_is_serilizable_to_output_stream();
				test_jwm_menu_serialization_includes_icon_extension_when_present();
				test_jwm_menu_serialization_has_unclassified_entries_at_the_end();
				test_jwm_menu_serialization_skips_files_with_missing_content();
				return qunit.errors();
			};
		};
	}
}


int main()
{
	return amm::jwm::menu_test(std::cerr, QUnit::normal).run();
}
