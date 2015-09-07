#ifndef __AUTOBAN__INITIALIZATIONEXCEPTION__
#define __AUTOBAN__INITIALIZATIONEXCEPTION__

#include <iostream>
#include <exception>

using namespace std;

class InitializationException : public logic_error {
  using logic_error::logic_error;
};

#endif
