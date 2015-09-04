#ifndef __irc_simulator__InitializationException__
#define __irc_simulator__InitializationException__

#include <iostream>
#include <exception>

using namespace std;

class InitializationException : public logic_error {
  using logic_error::logic_error;
};

#endif
