#ifndef __AUTOBAHN__LUAEXCEPTION__
#define __AUTOBAHN__LUAEXCEPTION__

#include <iostream>
#include <exception>

#include <lua.hpp>

using namespace std;

class LuaException : public logic_error {
  using logic_error::logic_error;
};

#endif
