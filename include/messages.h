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

#ifndef AMM_MESSAGES_H_
#define AMM_MESSAGES_H_

#include <string>
#include <vector>

namespace amm {
namespace messages {

std::string help();
std::string version();
std::string homeNotSet();
std::string optionError();
std::string badSummaryType(const std::string &actual);
std::string autogeneratedByAmm();
std::string noValidDesktopEntryFiles();
std::string badCategoryFile(const std::string &file_name);
std::string outputPathBlockedByDirectory(const std::string &file_name);
std::string badOutputFile(const std::string &file_name);

} // namespace messages
} // namespace amm

#endif // AMM_MESSAGES_H_
