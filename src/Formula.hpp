#ifndef __AUTOBAN_FORMULA__
#define __AUTOBAN_FORMULA__

#include <iostream>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>
#include <lua.hpp>

#include "ConfigurationChecker.hpp"
#include "exceptions/LuaException.hpp"

class Formula {
private:
  YAML::Node _script_info;
  std::string _script_lang;
  lua_State * _lua;

  lua_State * getLuaState();

public:
  Formula (YAML::Node, std::string);
  ~Formula () {
    lua_close(_lua);
  };

  static Formula* getFormula(std::string, ConfigurationChecker);

  void execute();
};

#endif
