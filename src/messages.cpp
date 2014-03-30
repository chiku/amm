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

#include <string>
#include <vector>
#include <sstream>
#include <ctime>

#include "../config.h"
#include "messages.h"

std::string
amm::messages::help()
{
	std::stringstream stream;

	stream << PACKAGE_STRING <<  ", Copyright (C) 2014 Chirantan Mitra <chirantan.mitra@gmail.com>" << std::endl;
	stream << PACKAGE << " comes with ABSOLUTELY NO WARRANTY; for details refer COPYING." << std::endl;
	stream << "This is free software, and you are welcome to redistribute it" << std::endl;
	stream << "under certain conditions; Refer COPYING for details." << std::endl;
	stream << std::endl;

	stream << PACKAGE << " creates JWM's menu from (freedesktop) desktop files" << std::endl;
	stream << std::endl;

	stream << "Optional arguments:" << std::endl;
	stream << " -o, --output-file [FILE]    Outfile file [Default: ./automenu]" << std::endl;
	stream << " -i, --input-directories [DIRECTORIES]" << std::endl;
	stream << "                             Directories to scan for '.desktop' files" << std::endl;
	stream << "                               List is separated by colons" << std::endl;
	stream << "                               [Default: applications directories under " << std::endl;
	stream << "                                $XDG_DATA_DIRS & $XDG_DATA_DIRS]" << std::endl;
	stream << " -a, --append-png            Add '.png' to all icon file names" << std::endl;
	stream << "     --help                  Show this help" << std::endl;
	stream << "     --version               Show version information" << std::endl;
	stream << std::endl;
	stream << "Deprecated optional arguments:" << std::endl;
	stream << "  -s [DIRECTORIES]           Same as -i" << std::endl;
	stream << "  -h                         Same as --help" << std::endl;
	stream << "  -v                         Same as --version" << std::endl;
	stream << std::endl;

	stream << "Include the generated file in the rootmenu section of your ~/.jwmrc" << std::endl;
	stream << "More information at http://github.com/chiku/mjwm" << std::endl;
	stream << std::endl;

	return stream.str();
}

std::string
amm::messages::version()
{
	std::stringstream stream;
	stream << PACKAGE_STRING << std::endl;
	return stream.str();
}

std::string
amm::messages::option_error()
{
	std::stringstream stream;
	stream << "Please run "<< PACKAGE << " --help to see options" << std::endl;
	return stream.str();
}

std::string
amm::messages::autogenerated_by_amm()
{
	std::time_t rawtime;
	tm *timeinfo;
	char buffer[80];
	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);
	std::strftime(buffer, 80, "%c", timeinfo);

	std::stringstream stream;
	stream << "<!-- Autogenerated by " << PACKAGE_STRING << " (" << buffer << ") -->" << std::endl;
	return stream.str();
}

std::string
amm::messages::no_valid_desktop_files()
{
	return "No valid desktop files were found in any search directories.";
}

std::string
amm::messages::bad_category_file(const std::string file_name)
{
	std::stringstream stream;
	stream << "Couldn't open category file: " << file_name;
	return stream.str();
}

std::string
amm::messages::bad_output_file(const std::string file_name)
{
	std::stringstream stream;
	stream << "Couldn't open output file: " << file_name;
	return stream.str();
}
