#ifndef __AUTOBAHN__EXECUTE__
#define __AUTOBAHN__EXECUTE__

#include <sstream>

#include "Command.hpp"
#include "../LanguagesManager.hpp"

class Execute : public Command {
private:
  std::string _script_name;
  FormulaManager _formula_manager;

  void checkArguments(const std::vector<std::string>&);

public:
  Execute (const std::vector<std::string>&, const ConfigurationChecker&);
  ~Execute () {};

  void run();

};

#endif
