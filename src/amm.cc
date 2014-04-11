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

#include "amm.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "vectorx.h"
#include "messages.h"
#include "amm_options.h"
#include "command_line_options_parser.h"
#include "service/icon_service_interface.h"
#include "service/icon/scan.h"
#include "service/file_search.h"
#include "representation.h"
#include "stats.h"
#include "menu.h"
#include "transformer/jwm.h"

namespace amm {

void Amm::LoadCommandLineOption(int argc, char **argv) {
  options_ = CommandLineOptionsParser().Parse(argc, argv);
  if (!options_.is_parsed) {
    std::cerr << messages::OptionError();
    exit(2);
  }
  std::vector<std::string> deprecations = options_.deprecations;
  if (deprecations.size() > 0) {
    std::cerr << VectorX(deprecations).Join("\n") << "\tProceeding..." << std::endl;
  }
  if (options_.is_help) {
    std::cout << messages::Help();
    exit(0);
  }
  if (options_.is_version) {
    std::cout << messages::Version();
    exit(0);
  }
}

void Amm::ReadCategories() {
  std::string category_file_name = options_.category_file_name;
  std::vector<std::string> category_lines;

  if (category_file_name != "") {
    std::ifstream category_file(category_file_name.c_str());
    if (category_file.good()) {
      std::string line;
      while (std::getline(category_file, line)) {
        category_lines.push_back(line);
      }
      menu_.LoadCustomCategories(category_lines);
      category_file.close();
    } else {
      std::cerr << messages::BadCategoryFile(category_file_name) << std::endl;
      exit(1);
    }
  }
}

void Amm::RegisterIconService() {
  if (options_.is_iconize) {
    service::IconServiceInterface *icon_service = new service::icon::Scan();
    menu_.RegisterIconService(*icon_service);
  }
}

void Amm::ReadDesktopFiles() {
  std::vector<std::string> input_directory_names = options_.input_directory_names;

  service::FileSearch service;
  service.RegisterDirectoriesWithFallback(input_directory_names);
  service.Resolve();

  std::vector<std::string> bad_paths = service.BadPaths();
  if (bad_paths.size() > 0) {
    std::cerr << "These paths couldn't be opened: " << VectorX(bad_paths).Join(", ");
    std::cerr << std::endl << "Proceeding..." << std::endl;
  }
  desktop_file_names_ = service.DesktopFileNames();
}

void Amm::Populate() {
  menu_.Populate(desktop_file_names_);
  if (menu_.Summary().TotalParsedFiles() == 0) {
    std::cerr << messages::NoValidDesktopFiles() << std::endl;
    exit(1);
  }
  menu_.Sort();
}

void Amm::WriteOutputFile() {
  std::string output_file_name = options_.output_file_name;
  std::ofstream output_file(output_file_name.c_str());
  if (!output_file.good()) {
    std::cerr << messages::BadOutputFile(output_file_name) << std::endl;
    exit(1);
  }

  transformer::Jwm jwm_transformer;
  std::vector<RepresentationInterface*> representations = menu_.Representations();

  output_file << messages::AutogeneratedByAmm();

  for (std::vector<RepresentationInterface*>::const_iterator representation = representations.begin(); representation != representations.end(); ++representation) {
    output_file << (*representation)->visit(jwm_transformer) << std::endl;
  }

  for (std::vector<RepresentationInterface*>::iterator representation = representations.begin(); representation != representations.end(); ++representation) {
    delete *representation;
  }

  output_file.close();
}

void Amm::PrintSummary() {
  std::cout << menu_.Summary().NormalSummary();
}

} // namespace amm
