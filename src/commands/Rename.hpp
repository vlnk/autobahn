#ifndef __AUTOBAHN__RENAME__
#define __AUTOBAHN__RENAME__

#include <boost/filesystem.hpp>

#include "Command.hpp"
#include "../ConfigurationChecker.hpp"

namespace fs = boost::filesystem;

class Rename : public Command {
private:
  std::string _old_script_name;
  std::string _new_script_name;

  const ConfigurationChecker& _conf;

  void checkArguments(const std::vector<std::string>&);

public:
  Rename (const std::vector<std::string>& args, const ConfigurationChecker&);
  ~Rename () {};

  void run();
};

#endif
