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

#include <fstream>
#include <string>
#include <vector>

#include "utils.h"
#include "icon_service.h"
#include "desktop_file.h"
#include "subcategory.h"
#include "menu.h"

amm::menu::menu()
{
	_unclassified_subcategory = amm::subcategory("Others", "others", "Others");

	create_default_categories();
}

void
amm::menu::create_default_categories()
{
	_subcategories.clear();

	_subcategories.push_back(amm::subcategory("Settings",    "settings",    "Settings"   ));
	_subcategories.push_back(amm::subcategory("Accessories", "accessories", "Utility"    ));
	_subcategories.push_back(amm::subcategory("Development", "development", "Development"));
	_subcategories.push_back(amm::subcategory("Education",   "education",   "Education"  ));
	_subcategories.push_back(amm::subcategory("Games",       "games",       "Game"       ));
	_subcategories.push_back(amm::subcategory("Graphics",    "graphics",    "Graphics"   ));
	_subcategories.push_back(amm::subcategory("Internet",    "internet",    "Network"    ));
	_subcategories.push_back(amm::subcategory("Multimedia",  "multimedia",  "AudioVideo" ));
	_subcategories.push_back(amm::subcategory("Office",      "office",      "Office"     ));
	_subcategories.push_back(amm::subcategory("Science",     "science",     "Science"    ));
	_subcategories.push_back(amm::subcategory("System",      "system",      "System"     ));
}

void
amm::menu::load_custom_categories(std::vector<std::string> lines)
{
	_subcategories.clear();

	for (std::vector<std::string>::const_iterator line = lines.begin(); line != lines.end(); ++line) {
		if ((*line)[0] != '#') {
			std::vector<std::string> tokens = amm::stringx(*line).split(":");
			if (tokens.size() >= 3 && tokens[0] != "" && tokens[1] != "") {
				std::vector<std::string> classification_names;
				for (size_t i = 2; i < tokens.size(); ++i) {
					if (tokens[i] != "") {
						classification_names.push_back(tokens[i]);
					}
				}
				if (classification_names.size() > 0) {
					amm::subcategory subcategory(tokens[0], tokens[1], classification_names);
					_subcategories.push_back(subcategory);
				}
			}
		}
	}
}

void
amm::menu::register_icon_service(amm::icon_service icon_service)
{
	_icon_service = icon_service;
}

void
amm::menu::populate(std::vector<std::string> desktop_file_names)
{
	std::vector<std::string>::const_iterator name;
	for (name = desktop_file_names.begin(); name != desktop_file_names.end(); ++name) {
		std::string line;
		amm::desktop_file desktop_file;
		std::ifstream file(name->c_str());

		if (file.good()) {
			while (std::getline(file, line)) {
				desktop_file.populate(line);
			}
			if (!desktop_file.display()) {
				_stats.add_suppressed_file(*name);
			} else if (desktop_file.is_valid()) {
				classify(desktop_file, *name);
			} else {
				_stats.add_unparsed_file(*name);
			}

			file.close();
		}
	}

	_subcategories.push_back(_unclassified_subcategory);
}

// TODO : desktop file should store which file it was created from
void
amm::menu::classify(amm::desktop_file desktop_file, std::string desktop_file_name)
{
	bool classified = false;

	std::vector<amm::subcategory>::iterator subcategory;
	for (subcategory = _subcategories.begin(); subcategory != _subcategories.end(); ++subcategory) {
		if (desktop_file.is_any_of(subcategory->classification_names())) {
			classified = true;
			subcategory->add_desktop_file(desktop_file);
		}
	}

	if (classified) {
		_stats.add_classified_file(desktop_file_name);
	} else {
		_unclassified_subcategory.add_desktop_file(desktop_file);
		_stats.add_unclassified_file(desktop_file_name);
		_stats.add_unhandled_classifications(desktop_file.categories());
	}
}

std::vector<amm::subcategory>
amm::menu::subcategories() const
{
	return _subcategories;
}

amm::stats
amm::menu::stats() const
{
	return _stats;
}

void
amm::menu::sort()
{
	std::vector<amm::subcategory>::iterator group;
	for (group = _subcategories.begin(); group != _subcategories.end(); ++group) {
		group->sort_desktop_files();
	}
}

std::vector<amm::representation::base*>
amm::menu::representations() const
{
	std::vector<amm::representation::base*> representations;
	amm::representation::menu_start *menu_start = new amm::representation::menu_start;
	representations.push_back(menu_start);

	std::vector<amm::subcategory>::const_iterator subcategory;
	for (subcategory = _subcategories.begin(); subcategory != _subcategories.end(); ++subcategory) {
		if (subcategory->has_entries()) {
			amm::representation::subcategory_start *start = new amm::representation::subcategory_start(subcategory->display_name(), _icon_service.resolved_name(subcategory->icon_name()));
			representations.push_back(start);

			std::vector<amm::desktop_file> desktop_files = subcategory->desktop_files();
			std::vector<amm::desktop_file>::const_iterator desktop_file;
			for (desktop_file = desktop_files.begin(); desktop_file != desktop_files.end(); ++desktop_file) {
				amm::representation::menu_entry *entry = new amm::representation::menu_entry(desktop_file->name(), _icon_service.resolved_name(desktop_file->icon()), desktop_file->executable());
				representations.push_back(entry);
			}

			amm::representation::subcategory_end *end = new amm::representation::subcategory_end(subcategory->display_name());
			representations.push_back(end);
		}
	}

	amm::representation::menu_end *menu_end = new amm::representation::menu_end;
	representations.push_back(menu_end);
	return representations;
}
