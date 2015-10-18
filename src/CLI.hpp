#ifndef __AUTOBAHN__CLI__
#define __AUTOBAHN__CLI__

#include <iostream>
#include <string>
#include <vector>

#include "Color.hpp"
#include "ConfigurationChecker.hpp"

#include "commands/Push.hpp"
#include "commands/Rename.hpp"
#include "commands/Execute.hpp"

#include "exceptions/CLIException.hpp"

namespace CLI {

  #ifdef PROJECT_NAME
    std::string prog_name(PROJECT_NAME);
  #else
    std::string prog_name("autobahn");
  #endif

  void help() {
    Color::Painter def(Color::FG_DEFAULT);
    Color::Painter opt(Color::FG_BLUE);

    std::cout << opt << "help | -h" << def << "\tprint help" << std::endl;
    std::cout << opt << "list | -l" << def << "\tlist all scripts (sort by language)" << std::endl;
    std::cout << opt << "get | -p <file> <name> <lang>" << def;
    std::cout << "\tpush file script into scripts directory" << std::endl;
    std::cout << opt << "export | -e <file>" << def << "\texport scripts NOT WORKING" << std::endl;
    std::cout << opt << "exec | -x <script name>" << def << "\texecute script in current directory" << std::endl;
    std::cout << opt << "clean | -c" << def << "\tclean all data and configuration." << std::endl;
    std::cout << opt << "init | -i <*config dir> <*script dir>" << def << "\t(default: ~/.autobahn ~/Scripts)" << std::endl;
  }

  void usage() {
    Color::Painter def(Color::FG_DEFAULT);
    Color::Painter use(Color::FG_YELLOW);

    std::cout << use << "usage: " << def << CLI::prog_name << ' ' << "[option]" << " <*arguments>" << std::endl;
  }

  void parseCommandLine(const std::vector<std::string>& args) {
    ConfigurationChecker checker;

    if (args.size() == 0) {
        throw CLIException("no arguments, try help");
        usage();
    }

    if (!checker.isInitialized() && (args[0].compare("init") != 0)) {
      throw InitializationException("the configuration is not initialised!");
    }

    /*** CLEAN ***/
    if (args[0].compare("clean") == 0) {
      checker.clean();
    }

    /*** EXEC ***/
    else if (args[0].compare("exec") == 0) {
      Execute e = Execute(args, checker);
      e.run();
    }

    /*** HELP ***/
    else if (args[0].compare("help") == 0) {
      usage();
      help();
    }

    /*** INITIALIZATION ***/
    else if (args[0].compare("init") == 0) {
      checker.initializeConfiguration(args);
    }

    /*** LIST ***/
    else if (args[0].compare("list") == 0) {
      checker.list();
    }

    /*** PUSH ***/
    else if (args[0].compare("get") == 0) {
      Push p = Push(args, checker);
      std::cout << p << std::endl;
      p.run();
    }

    /*** RENAME ***/
    else if (args[0].compare("rename") == 0) {
      Rename r = Rename(args, checker);
      r.run();
    }
  }
}

#endif
