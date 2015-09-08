#ifndef __AUTOBAHN_CONFIGURATIONCHECKER__
#define __AUTOBAHN_CONFIGURATIONCHECKER__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <yaml-cpp/yaml.h>

#include "Color.hpp"
#include "exceptions/InitializationException.hpp"

namespace fs = boost::filesystem;

class ConfigurationChecker {
private:
  const std::string HOME = "HOME";
  const std::string _default_scripts_dir = "Scripts";
  const std::string _default_config_dir = ".autobahn";
  const std::string _config_file = "config.yaml";
  const std::string _data_file = "data.yaml";

  fs::path _config_dir;
  fs::path _scripts_dir;
  fs::path _languages_dir;

  const std::vector<const std::string> getLanguages() const;

public:
  ConfigurationChecker ();
  ~ConfigurationChecker () {};

  bool isInitialized();
  void initializeConfiguration(std::vector<std::string>);

  YAML::Node loadData() const {
    fs::path data_file_path = fs::path(_config_dir).append('/' + _data_file);
    return YAML::LoadFile(data_file_path.string());
  }

  YAML::Node loadConfig() const {
    fs::path config_file_path = fs::path(_config_dir).append('/' + _config_file);
    return YAML::LoadFile(config_file_path.string());
  }

  void appendData(const YAML::Node&) const;
  void updateData(const YAML::Node&) const;

  void clean();
  void list();

  bool isValidLanguage(const std::string&) const;
  bool isValidPath(const fs::path&) const;
  bool isValidScript(const std::string&) const;

  const fs::path& getScriptDirPath() const { return _scripts_dir; };
  const fs::path& getLangDirPath() const { return _languages_dir; };
  const YAML::Node getScriptInfo(const std::string&) const;

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
