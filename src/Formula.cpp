#include "Formula.hpp"

Formula::Formula(YAML::Node script_info, std::string script_lang) :
  _script_info(script_info),
  _script_lang(script_lang) {

  _lua = getLuaState();

  execute();
}

void Formula::execute() {
  const char* script_path = _script_info["path"].as<std::string>().c_str();
  lua_getfield(_lua, -1, "execute");
  lua_pushvalue(_lua, -2);
  lua_pushstring(_lua, script_path);

  int result = lua_pcall(_lua, 2, 0, 0);
  if (result) {
    throw LuaException("execute call failed");
  }
}

lua_State* Formula::getLuaState() {
  lua_State * _lua = luaL_newstate();
  if (_lua == NULL) {
    throw LuaException("unable to load lua state");
  }

  luaL_openlibs(_lua); /* Load Lua libraries */

  /* Load the file containing the script we are going to run */
  int status = luaL_dofile(_lua, "../formulas/perl.lua");
  if (status) {
    std::string lua_load_file_error(lua_tostring(_lua, -1));
    throw LuaException("couldn't load file: " + lua_load_file_error);
  }

  /* By what name is the script going to reference our table? */
  lua_getfield(_lua, -1, "new");

  /* Verify that new is a function. */
  if (!lua_isfunction(_lua, -1)) {
    std::string lua_get_function_error(lua_tostring(_lua, -1));
    throw LuaException("invalid lua function: " + lua_get_function_error);
  }

  /* Move the counter module to be the first argument of new. */
  lua_insert(_lua, -2);

  /* Call new(M, start). 2 arguments. 2 return values. */
  int result = lua_pcall(_lua, 1, 2, 0);
  if (result) {
    std::string lua_call_function_error(lua_tostring(_lua, -1));
    throw LuaException("error in function calling: " + lua_call_function_error);
  }

  if (lua_type(_lua, -2) == LUA_TNIL) {
    std::string lua_protocol_error(lua_tostring(_lua, -1));
    throw LuaException("invalid protocol: " + lua_protocol_error);
  }
  lua_pop(_lua, 1);

  if (lua_type(_lua, -1) != LUA_TTABLE) {
    std::string lua_type_error(lua_tostring(_lua, -1));
    throw LuaException("Invalid type (%d) returned by new:" + lua_type_error);
  }

  /* We're sure we have a table returned by new.
   * This is the only item on the stack right now. */
  return _lua;
}

Formula* Formula::getFormula(std::string script_name, ConfigurationChecker conf) {
  std::string script_lang;
  YAML::Node data_tree = conf.loadData();

  for (auto it = data_tree.begin(); it != data_tree.end(); it++) {
    if ((it->second).IsSequence()) {

      script_lang = (it->first).as<std::string>();

      for (auto it_seq = (it->second).begin(); it_seq != (it->second).end(); it_seq++) {
        if (script_name.compare((*it_seq)["name"].as<std::string>()) == 0) {
          //std::cout << "found: " << (*it_seq)["name"] << std::endl;
          //std::cout << "lang: " << script_lang << std::endl;

          return new Formula(*it_seq, script_lang);
        }
      }
    }
  }
  return NULL;
}
