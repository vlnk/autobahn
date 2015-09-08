#ifndef __AUTOBAHN__COMMAND__
#define __AUTOBAHN__COMMAND__

#include <string>
#include <vector>

class Command {
protected:
  virtual void checkArguments(const std::vector<std::string>&) = 0;

public:
  Command () {};
  virtual ~Command () {};

  virtual void run() = 0;
};

#endif
