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

#ifndef AMM_ENVIRONMENT_VARIABLE_H_
#define AMM_ENVIRONMENT_VARIABLE_H_

#include <string>
#include <vector>

namespace amm {
class EnvironmentVariable {
 public:
  EnvironmentVariable();
  std::vector<std::string> ApplicationBaseDirectories() const;
  std::string Home() const { return home_; }
  std::string XdgDataHome() const { return xdg_data_home_; }
  std::vector<std::string> XdgDataDirectories() const;

 private:
  std::string home_;
  std::string xdg_data_home_;
  std::string xdg_data_dirs_;
};
} // namespace amm

#endif // AMM_ENVIRONMENT_VARIABLE_H_
