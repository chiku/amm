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

#include "stringx.h"
#include "filex.h"
#include "icon_search/icon_search_interface.h"
#include "icon_search/mirror_search.h"
#include "xdg/desktop_entry.h"
#include "subcategory.h"
#include "representation/menu_start.h"
#include "representation/menu_end.h"
#include "representation/subcategory_start.h"
#include "representation/subcategory_end.h"
#include "representation/program.h"

namespace amm {

Menu::Menu() : icon_searcher_(std::unique_ptr<icon_search::IconSearchInterface>{new icon_search::MirrorSearch}), unclassified_subcategory_(Subcategory::Others())
{
    createDefaultCategories();
}

void Menu::registerIconService(icon_search::IconSearchInterface *icon_searcher)
{
    icon_searcher_.reset(icon_searcher);
}

void Menu::createDefaultCategories()
{
    subcategories_ = {
        Subcategory::Settings(),
        Subcategory::Accessories(),
        Subcategory::Development(),
        Subcategory::Education(),
        Subcategory::Games(),
        Subcategory::Graphics(),
        Subcategory::Internet(),
        Subcategory::Multimedia(),
        Subcategory::Office(),
        Subcategory::Science(),
        Subcategory::System(),
    };
}

void Menu::loadCustomCategories(const std::vector<std::string> &lines)
{
    subcategories_.clear();

    for (const auto &line : lines) {
        if (line[0] != '#') {
            auto tokens = StringX(StringX(line).trim()).split(":");
            if (tokens.size() >= 3 && tokens[0] != "" && tokens[1] != "") {
                std::vector<std::string> classification_names;
                for (auto token = tokens.begin()+2; token != tokens.end(); ++token) {
                    if (*token != "") {
                        classification_names.push_back(*token);
                    }
                }
                if (classification_names.size() > 0) {
                    subcategories_.push_back(Subcategory(tokens[0], tokens[1], classification_names));
                }
            }
        }
    }
}

void Menu::populate(const std::vector<std::string> &entry_names)
{
    for (const auto &name : entry_names) {
        addDesktopEntry(name);
    }

    subcategories_.push_back(unclassified_subcategory_);
}

void Menu::addDesktopEntry(const std::string &entry_name)
{
    std::vector<std::string> lines;
    if (!FileX(entry_name).readLines(&lines)) {
        summary_.addUnparsedFile(entry_name);
        return;
    }
    xdg::DesktopEntry entry;
    entry.hasLanguage(language_);
    entry.parse(lines);

    if (!entry.display()) {
        summary_.addSuppressedFile(entry_name);
        return;
    }

    if (!entry.isValid()) {
        summary_.addUnparsedFile(entry_name);
        return;
    }

    bool classified = classify(entry);
    if (classified) {
        summary_.addClassifiedFile(entry_name);
    } else {
        unclassified_subcategory_.addDesktopEntry(entry);
        summary_.addUnclassifiedFile(entry_name);
        summary_.addUnhandledClassifications(entry.categories());
    }
}

bool Menu::classify(const xdg::DesktopEntry &entry)
{
    bool classified = false;

    for (auto &subcategory : subcategories_) {
        if (entry.isAnyOf(subcategory.classificationNames())) {
            classified = true;
            subcategory.addDesktopEntry(entry);
        }
    }

    return classified;
}

void Menu::sort()
{
    for (auto &group : subcategories_) {
        group.sortDesktopEntries();
    }
}

std::vector<std::unique_ptr<representation::RepresentationInterface>> Menu::representations() const
{
    std::vector<std::unique_ptr<representation::RepresentationInterface>> representations;
    auto menu_start = std::unique_ptr<representation::RepresentationInterface> { new representation::MenuStart };
    representations.push_back(std::move(menu_start));

    for (const auto &subcategory : subcategories_) {
        if (subcategory.hasEntries()) {
            auto icon_name = icon_searcher_->resolvedName(subcategory.iconName());
            auto start = std::unique_ptr<representation::RepresentationInterface> { new representation::SubcategoryStart(subcategory.displayName(), icon_name) };
            representations.push_back(std::move(start));

            auto entries = subcategory.desktopEntries();
            for (const auto &entry : entries) {
                auto icon_name = icon_searcher_->resolvedName(entry.icon());
                auto program = std::unique_ptr<representation::RepresentationInterface> { new representation::Program(entry.name(), icon_name, entry.executable(), entry.comment()) };
                representations.push_back(std::move(program));
            }

            auto end = std::unique_ptr<representation::RepresentationInterface> { new representation::SubcategoryEnd(subcategory.displayName()) };
            representations.push_back(std::move(end));
        }
    }

    auto menu_end = std::unique_ptr<representation::RepresentationInterface> { new representation::MenuEnd };
    representations.push_back(std::move(menu_end));
    return representations;
}

} // namespace amm
