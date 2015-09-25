#ifndef __AUTOBAHN_FORMULAMANAGER__
#define __AUTOBAHN_FORMULAMANAGER__

#include <iostream>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>
#include <lua.hpp>

#include "ConfigurationChecker.hpp"
#include "exceptions/LuaException.hpp"

class FormulaManager {
private:
  lua_State * _lua_state;

public:
  FormulaManager () {
    _lua_state = luaL_newstate();

    if (_lua_state == NULL) {
      throw LuaException("unable to load lua state");
    }

    luaL_openlibs(_lua_state); /* Load Lua libraries */
  };

  ~FormulaManager () { lua_close(_lua_state); };

  void getLuaStateForLanguages(std::string, std::string);

  void execute(std::string);
};

#endif
