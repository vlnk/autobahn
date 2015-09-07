#ifndef __AUTOBAN__LUAEXCEPTION__
#define __AUTOBAN__LUAEXCEPTION__

#include <iostream>
#include <exception>

#include <lua.hpp>

using namespace std;

class LuaException : public logic_error {
  using logic_error::logic_error;
};

#endif
