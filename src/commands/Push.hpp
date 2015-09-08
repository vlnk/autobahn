#ifndef __AUTOBAHN_PUSH__
#define __AUTOBAHN_PUSH__

#include <iostream>
#include <string>
#include <vector>
#include <regex>

#include <boost/filesystem.hpp>
#include <yaml-cpp/yaml.h>

#include "Command.hpp"
#include "../Color.hpp"
#include "../ConfigurationChecker.hpp"

namespace fs = boost::filesystem;

class Push : public Command {
private:
  std::string _script_file;
  std::string _script_name;
  std::string _script_lang;
  const ConfigurationChecker& _conf;

  YAML::Node getMetaData(const fs::path&);
  void checkArguments(const std::vector<std::string>&);

public:
  Push(const std::vector<std::string>&, const ConfigurationChecker&);
  ~Push() {};

  void run();

  friend std::ostream& operator << (std::ostream& os, const Push& push) {
    Color::Painter def(Color::FG_DEFAULT);
    Color::Painter opt(Color::FG_BLUE);

    os << opt << "script file: " << def;
    os << push._script_file << std::endl;
    os << opt << "script name: " << def;
    os << push._script_name << std::endl;
    os << opt << "script language: " << def;
    os << push._script_lang << std::endl;

    return os;
  };
};

#endif
