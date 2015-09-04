#ifndef __script_center_ConfigurationChecker__
#define __script_center_ConfigurationChecker__

#include <iostream>
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
#include <yaml-cpp/yaml.h>

#include "Color.hpp"
#include "exceptions/InitializationException.hpp"

using namespace boost::filesystem;

class ConfigurationChecker {
private:
  path _config_dir;
  path _scripts_dir;

public:
  ConfigurationChecker ();
  ~ConfigurationChecker () {};

  bool isInitialized();
  void initializeConfiguration(std::vector<std::string>);

  YAML::Node loadData() const;
  void saveData(const YAML::Node&) const;

  void clean();
  void list();

  const path& getScriptDirPath() const { return _scripts_dir; };

  friend std::ostream& operator << (std::ostream& os, const ConfigurationChecker& conf) {
    Color::Painter def(Color::FG_DEFAULT);
    Color::Painter opt(Color::FG_BLUE);

    os << opt << "configuration directory: " << def;
    os << conf._config_dir << std::endl;
    os << opt << "scripts directory: " << def;
    os << conf._scripts_dir << std::endl;

    return os;
  };
};

#endif
