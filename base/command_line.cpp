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

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iostream>
#include <algorithm>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "base/ostreamfmt.h"
#include "base/command_line.h"


namespace basepp {

  namespace command_line {

    parser::parser (const std::string& app_name, std::initializer_list<arg> args)
      : app_name(app_name)
      , commands(args)
    {
      commands.emplace_back(
        arg{"-h", "--help", {}, "This help", [&](const std::string&) {
           show_help(std::cout);
           exit(0);
         }}
      );
    }

    void parser::add (std::initializer_list<arg> args) {
      commands.insert(commands.end(), args);
    }

    void parser::add (arg&& a) {
      commands.emplace_back(std::move(a));
    }

    void parser::process (int argc, const char* argv[]) {
      arg_list args;
      for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
      }
      process(args);
    }

    void parser::process (const arg_list& args) {
      for (iterator i = args.begin(), e = args.end(); i != e; ++i) {
        bool found = false;
        for (const arg& cmd : commands) {
          if (basepp::string::starts_with(*i, cmd.short_cmd)) {
            execute(cmd, cmd.short_cmd, i, e);
            found = true;
            break;
          } else if (basepp::string::starts_with(*i, cmd.long_cmd)) {
            execute(cmd, cmd.long_cmd, i, e);
            found = true;
            break;
          }
        }
        if (!found) {
          throw std::invalid_argument(ostreamfmt("Unknown argument: " << *i));
        }
      }
    }

    void parser::show_help (std::ostream& out) {
      out << app_name << std::endl
          << "available options:"<< std::endl;
      std::string::size_type min_len = 0;
      for (const arg& arg: commands) {
        min_len = std::max(min_len, arg.short_cmd.size() + arg.needed_arg.size() * 2 + 3 + arg.long_cmd.size());
      }
      for (const arg& arg: commands) {
        size_t len = arg.short_cmd.size() + arg.needed_arg.size() * 2 + 3 + arg.long_cmd.size();
        out << arg.short_cmd << arg.needed_arg << " | " << arg.long_cmd << arg.needed_arg;
        while (len < min_len) {
          out << ' ';
          ++len;
        }
        out << " : " << arg.explanation << std::endl;
      }
    }

    void parser::execute (const arg& cmd, const std::string& match, iterator& i, const iterator& e) {
      if (cmd.needed_arg.empty()) {
          cmd.action({});
      } else {
        if (i->length() > match.length()) {
          cmd.action(i->substr(match.length()));
        } else if (i < e) {
          ++i;
          cmd.action(*i);
        } else {
          throw std::invalid_argument(ostreamfmt("missing argument after " << match));
        }
      }
    }

  } // namespace command_line

} // basepp
