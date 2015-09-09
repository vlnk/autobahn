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

namespace CLI {
  std::string prog_name("autobahn");

  void help() {
    Color::Painter def(Color::FG_DEFAULT);
    Color::Painter opt(Color::FG_BLUE);

    std::cout << opt << "\thelp | -h" << def << " print help" << std::endl;
    std::cout << opt << "\tlist | -l" << def << " list all scripts (sort by language)" << std::endl;
    std::cout << opt << "\tpush | -p <file> <name> <lang>" << def;
    std::cout << " push file script into scripts directory" << std::endl;
    std::cout << opt << "\texport | -e <file>" << def << " export scripts NOT WORKING" << std::endl;
    std::cout << opt << "\texec | -x <script name>" << def << " execute script in current directory" << std::endl;
    std::cout << opt << "\tclean | -c" << def << " clean all data and configuration." << std::endl;
    std::cout << opt << "\tinit | -i <*config dir> <*script dir>" << def << " (default: ~/.autoban ~/Scripts)" << std::endl;
  }

  void usage() {
    Color::Painter def(Color::FG_DEFAULT);
    Color::Painter use(Color::FG_YELLOW);

    std::cout << use << "usage: " << def << CLI::prog_name << ' ' << "[option]" << " <*arguments>" << std::endl;
  }

  void parseCommandLine(const std::vector<std::string>& args) {
    ConfigurationChecker checker;

    if (!checker.isInitialized()) {
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
    else if (args[0].compare("push") == 0) {
      Push p = Push(args, checker);
      std::cout << p << std::endl;
      p.run();
    }

    /*** RENAME ***/
    else if (args[0].compare("rename") == 0) {
      Rename r = Rename(args, checker);
      r.run();
    }

    else {
      usage();
    }
  }
}

#endif
