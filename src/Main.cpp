#include <iostream>
#include <string>
#include <vector>

#include "Color.hpp"
#include "CLI.hpp"

#include "exceptions/InitializationException.hpp"

int main(int argc, char const *argv[]) {
  Color::Painter def(Color::FG_DEFAULT);
  Color::Painter war(Color::FG_YELLOW);
  Color::Painter err(Color::FG_RED);

  std::vector<std::string> args(argv + 1, argv + argc);

  try {
    CLI::parseCommandLine(args);
  }
  catch (std::invalid_argument& e) {
    std::cout << err << e.what() << def << std::endl;
    CLI::usage();
  }
  catch (InitializationException& e) {
    std::cout << err << e.what() << def << std::endl;
    std::cout << "use <init> command to configure directories" << std::endl;
    std::cout << std::endl;
    CLI::usage();
    CLI::help();
  }
  catch (std::logic_error& e) {
    std::cerr << err << e.what() << def << std::endl;
  }
  catch(std::exception& e) {
    std::cerr << err << e.what() << def << std::endl;
  }

  return 0;
}
