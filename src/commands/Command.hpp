#ifndef __AUTOBAHN__COMMAND__
#define __AUTOBAHN__COMMAND__

#include <string>
#include <vector>
#include "../ConfigurationChecker.hpp"

class Command {
protected:
  const ConfigurationChecker& _conf;
  virtual void checkArguments(const std::vector<std::string>&) = 0;

public:
  Command (const ConfigurationChecker& conf):_conf(conf) {};
  virtual ~Command () {};

  virtual void run() = 0;
};

#endif
