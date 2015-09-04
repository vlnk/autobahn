#ifndef __script_center_Push__
#define __script_center_Push__

#include <iostream>
#include <string>
#include <vector>
#include <regex>

#include <boost/filesystem.hpp>
#include <yaml-cpp/yaml.h>

#include "../Color.hpp"
#include "../ConfigurationChecker.hpp"

class Pusher {
private:
  std::string _script_file;
  std::string _script_name;
  std::string _script_lang;

  YAML::Node getMetaData(const path&);

public:
  Pusher(std::string script_file, std::string script_name, std::string script_lang):
    _script_file(script_file),
    _script_name(script_name),
    _script_lang(script_lang) {};

  ~Pusher() {};

  static Pusher* checkPushArguments(std::vector<std::string>);
  void push(const ConfigurationChecker&);

  friend std::ostream& operator << (std::ostream& os, const Pusher& push) {
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
