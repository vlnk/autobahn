#include "FormulaManager.hpp"

void FormulaManager::execute(std::string script_path) {
  lua_getfield(_lua_state, -1, "execute");
  lua_pushvalue(_lua_state, -2);
  lua_pushstring(_lua_state, script_path.c_str());

  int result = lua_pcall(_lua_state, 2, 0, 0);
  if (result) {
    throw LuaException("execute call failed");
  }
}

void FormulaManager::getLuaStateForLanguages(std::string lang_dir, std::string script_lang) {

  /* Load the file containing the script we are going to run */
  std::string file_lang;
  std::transform(script_lang.begin(), script_lang.end(), std::back_inserter(file_lang), ::tolower);

  std::string file_name = lang_dir + '/' + file_lang + ".lua";

  int status = luaL_dofile(_lua_state, file_name.c_str());
  if (status) {
    std::string lua_load_file_error(lua_tostring(_lua_state, -1));
    throw LuaException("couldn't load file: " + lua_load_file_error);
  }

  /* By what name is the script going to reference our table? */
  lua_getfield(_lua_state, -1, "new");

  /* Verify that new is a function. */
  if (!lua_isfunction(_lua_state, -1)) {
    std::string lua_get_function_error(lua_tostring(_lua_state, -1));
    throw LuaException("invalid lua function: " + lua_get_function_error);
  }

  /* Move the counter module to be the first argument of new. */
  lua_insert(_lua_state, -2);

  /* Call new(M, start). 2 arguments. 2 return values. */
  int result = lua_pcall(_lua_state, 1, 2, 0);
  if (result) {
    std::string lua_call_function_error(lua_tostring(_lua_state, -1));
    throw LuaException("error in function calling: " + lua_call_function_error);
  }

  if (lua_type(_lua_state, -2) == LUA_TNIL) {
    std::string lua_protocol_error(lua_tostring(_lua_state, -1));
    throw LuaException("invalid protocol: " + lua_protocol_error);
  }
  lua_pop(_lua_state, 1);

  if (lua_type(_lua_state, -1) != LUA_TTABLE) {
    std::string lua_type_error(lua_tostring(_lua_state, -1));
    throw LuaException("Invalid type (%d) returned by new:" + lua_type_error);
  }
}
