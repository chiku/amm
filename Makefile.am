## Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = subdir-objects

# AM_CPPFLAGS=-I$(top_builddir)/include -I$(top_srcdir)/include -Wall -Wextra -fno-exceptions -fno-rtti -fno-threadsafe-statics -ffast-math -flto
AM_CPPFLAGS=-I$(top_builddir)/include -I$(top_srcdir)/include -Wall -Wextra -std=c++11

bin_PROGRAMS = amm

implementation_files = src/stringx.cc \
                       src/vectorx.cc \
                       src/filex.cc \
                       src/directoryx.cc \
                       src/xdg/entry_line.cc \
                       src/xdg/entry.cc \
                       src/xdg/desktop_entry.cc \
                       src/xdg/icon_subdirectory.cc \
                       src/xdg/icon_theme.cc \
                       src/subcategory.cc \
                       src/stats.cc \
                       src/menu.cc \
                       src/transformer/jwm.cc \
                       src/system_environment.cc \
                       src/desktop_entry_file_search.cc \
                       src/amm_options.cc \
                       src/command_line_options_parser.cc

test_files = test/stringx_test.cc \
             test/vectorx_test.cc \
             test/filex_test.cc \
             test/directoryx_test.cc \
             test/xdg/entry_line_test.cc \
             test/xdg/entry_test.cc \
             test/xdg/desktop_entry_test.cc \
             test/xdg/icon_subdirectory_test.cc \
             test/xdg/icon_theme_test.cc \
             test/subcategory_test.cc \
             test/stats_test.cc \
             test/menu_test.cc \
             test/transformer/jwm_test.cc \
             test/system_environment_test.cc \
             test/desktop_entry_file_search_test.cc \
             test/amm_options_test.cc \
             test/command_line_options_parser_test.cc \
             test/icon_search/caching_search_test.cc

amm_SOURCES = $(implementation_files) src/timex.cc src/messages.cc src/amm.cc src/qualified_icon_theme.cc src/icon_search/xdg_search.cc src/main.cc

check_PROGRAMS = amm_test

ammdir=$(datadir)/amm
amm_DATA = data/default.amm data/puppy.amm
man1_MANS = doc/amm.1

TESTS = $(check_PROGRAMS)

amm_test_SOURCES = $(implementation_files) $(test_files) test/test_runner.cc
