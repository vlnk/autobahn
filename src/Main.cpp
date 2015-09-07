#include <iostream>
#include <string>
#include <vector>

#include "Color.hpp"
#include "ConfigurationChecker.hpp"
#include "commands/Push.hpp"
#include "exceptions/InitializationException.hpp"
#include "Formula.hpp"

void usage(std::string prog_name) {
  Color::Painter def(Color::FG_DEFAULT);
  Color::Painter use(Color::FG_YELLOW);

  std::cout << use << "usage: " << def << prog_name << ' ' << "[option]" << " <*arguments>" << std::endl;
}

void help(std::string prog_name) {
  Color::Painter def(Color::FG_DEFAULT);
  Color::Painter opt(Color::FG_BLUE);

  std::cout << opt << "help | -h" << def << " print help" << std::endl;
  std::cout << opt << "list | -l" << def << " list all scripts (sort by language)" << std::endl;
  std::cout << opt << "push | -p <file> <name> <lang>" << def;
  std::cout << " push file script into scripts directory" << std::endl;
  std::cout << opt << "export | -e <file>" << def << " export scripts NOT WORKING" << std::endl;
  std::cout << opt << "exec | -x <script name>" << def << " execute script in current directory" << std::endl;
  std::cout << opt << "clean | -c" << def << " clean all data and configuration." << std::endl;
  std::cout << opt << "init | -i <*config dir> <*script dir>" << def << " (default: ~/.autoban ~/Scripts)" << std::endl;
}

int main(int argc, char const *argv[]) {
  Color::Painter def(Color::FG_DEFAULT);
  Color::Painter war(Color::FG_YELLOW);
  Color::Painter err(Color::FG_RED);

  if (argc < 2) {
    usage(argv[0]);
  }
  else {
    std::vector<std::string> args(argv + 1, argv + argc);
    std::string prog_name("autobahn");

    try {
      ConfigurationChecker checker;

      /*** INITIALIZATION ***/
      if ((args[0].compare("init") == 0) || (args[0].compare("-i")) == 0) {
        checker.initializeConfiguration(args);
      }
      else if (!checker.isInitialized()) {
        throw InitializationException("the configuration is not initialised!");
      }

      /*** HELP ***/
      else if ((args[0].compare("help") == 0) || (args[0].compare("-h")) == 0) {
        usage(prog_name);
        help(prog_name);
      }

      /*** PUSH ***/
      else if ((args[0].compare("push") == 0) || (args[0].compare("-p")) == 0) {
        Pusher* pusher = Pusher::checkPushArguments(args);
        std::cout << *pusher;

        pusher->push(checker);
        delete pusher;
      }

      /*** CLEAN ***/
      else if ((args[0].compare("clean") == 0) || (args[0].compare("-c")) == 0) {
        checker.clean();
      }

      /*** LIST ***/
      else if ((args[0].compare("list") == 0) || (args[0].compare("-l")) == 0) {
        checker.list();
      }

      /*** EXEC ***/
      else if ((args[0].compare("exec") == 0) || (args[0].compare("-x")) == 0) {
        if (args.size() != 2) {
          throw std::invalid_argument("Incorrect 'Exec' parameters.");
        }

        try {
          auto formula = Formula::getFormula(args[1], checker);
        }
        catch (LuaException& e) {
          std::cerr << err << e.what() << def << std::endl;
        }
      }

      /*** USAGE ***/
      else {
        usage(prog_name);
      }
    }
    catch (std::invalid_argument& e) {
      std::cout << err << e.what() << def << std::endl;
      usage(prog_name);
    }
    catch (InitializationException& e) {
      std::cout << err << e.what() << def << std::endl;
      std::cout << "use <init> command to configure directories" << std::endl;
      std::cout << std::endl;
      usage(prog_name);
      help(prog_name);
    }
    catch(std::exception& e) {
      std::cerr << err << e.what() << def << std::endl;
    }
  }

  return 0;
}
