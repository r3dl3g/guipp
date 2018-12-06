/**
 * @copyright (c) 2018-2018 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    standard lib
 *
 * Customer   -
 *
 * @brief     C++ API: command_line
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <functional>
#include <vector>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "string_util.h"


namespace basepp {

  namespace command_line {

    struct arg {
      std::string short_cmd;
      std::string long_cmd;
      std::string needed_arg;
      std::string explanation;
      std::function<void(std::string)> action;
    };

    struct parser {
      typedef std::vector<std::string> arg_list;
      typedef arg_list::const_iterator iterator;

      parser (const std::string& app_name = {}, std::initializer_list<arg> args = {});

      void add (std::initializer_list<arg> args);
      void add (arg&& a);

      void process (int argc, const char* argv[]);
      void process (const arg_list& args);

      void show_help (std::ostream&);

    private:
      void execute (const arg& cmd, const std::string& match, iterator& i, const iterator& e);

      std::string app_name;
      std::vector<arg> commands;
    };

  } // namespace command_line

} // namespace basepp
