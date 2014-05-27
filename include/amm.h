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

#ifndef AMM_AMM_H_
#define AMM_AMM_H_

#include <string>
#include <vector>

#include "environment_variable.h"
#include "amm_options.h"
#include "menu.h"

namespace amm {
class Amm {
 public:
  Amm();
  ~Amm();
  void ValidateEnvironment();
  void LoadCommandLineOption(int argc, char **argv);
  void RegisterIconService();
  void ReadCategories();
  void ReadDesktopEntryFiles();
  void Populate();
  void WriteOutputFile();
  void PrintSummary();

 private:
  EnvironmentVariable environment_variable_;
  AmmOptions *options_;
  Menu menu_;
  std::vector<std::string> desktop_entry_file_names_;
};
} // namespace amm

#endif // AMM_AMM_H_
