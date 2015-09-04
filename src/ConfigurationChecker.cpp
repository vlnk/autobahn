#include "ConfigurationChecker.hpp"

static const std::string HOME = "HOME";

const std::string default_scripts_dir = "Scripts";
const std::string default_config_dir = ".autoban";
const std::string config_file = "config.yaml";
const std::string data_file = "data.yaml";

const std::vector<std::string> languages {"PERL"};

ConfigurationChecker::ConfigurationChecker() {
  #ifdef CONFIG_DIR
    _config_dir = path(CONFIG_DIR);
  #else
    char const* tmp_config = getenv(HOME.c_str());
    if(tmp_config != NULL) {
      std::string home = std::string(tmp_config);
      _config_dir = path(home + '/' + default_config_dir);
    }
  #endif

  #ifdef SCRIPTS_DIR
    _scripts_dir = path(SCRIPTS_DIR);
  #else
    char const* tmp_scripts = getenv(HOME.c_str());
    if(tmp_scripts != NULL) {
      std::string home = std::string(tmp_scripts);
      _scripts_dir = path(home + '/' + default_scripts_dir);
    }
  #endif
}

bool ConfigurationChecker::isInitialized() {
  path config_file_path = path(_config_dir).append('/' + config_file);
  return exists(config_file_path);
}

void ConfigurationChecker::initializeConfiguration(std::vector<std::string> args) {
  if (args.size() == 3) {
    if (exists(args[1]) && exists(args[2])) {
      _config_dir = path(args[1]);
      _scripts_dir = path(args[2]);
    }
    else {
      throw InitializationException("incorects directories!");
    }
  }

  path config_file_path = path(_config_dir).append('/' + config_file);
  path data_file_path = path(_config_dir).append('/' + data_file);

  std::ofstream config_file, data_file;

  create_directory(_config_dir);
  create_directory(_scripts_dir);

  config_file.open(config_file_path.string());

  YAML::Emitter config_out;
  config_out << YAML::BeginMap;
  config_out << YAML::Key << "DIRECTORY";
  config_out << YAML::Value << _scripts_dir.string();
  config_out << YAML::EndMap;

  config_file << config_out.c_str();
  config_file.close();

  data_file.open(data_file_path.string());

  YAML::Emitter data_out;
  data_out << YAML::BeginMap;
  data_out << YAML::Key << "SIZE";
  data_out << YAML::Value << 0;
  data_out << YAML::EndMap;

  data_file << data_out.c_str();
  data_file.close();
}

YAML::Node ConfigurationChecker::loadData() const {
  path data_file_path = path(_config_dir).append('/' + data_file);
  return YAML::LoadFile(data_file_path.string());
}

void ConfigurationChecker::saveData(const YAML::Node& node) const {
  path data_file_path = path(_config_dir).append('/' + data_file);
  std::ofstream data_file;

  data_file.open(data_file_path.string());
  YAML::Emitter data_out;
  data_out << node;

  data_file << data_out.c_str();
  data_file.close();
}

void ConfigurationChecker::clean() {
  remove_all(_config_dir);
  remove_all(_scripts_dir);
}

void ConfigurationChecker::list() {
  Color::Painter def(Color::FG_DEFAULT);
  Color::Painter lang(Color::FG_YELLOW);
  YAML::Node data = loadData();

  for (auto it = data.begin(); it != data.end(); it++) {
    if (std::find(languages.begin(), languages.end(), it->first.as<std::string>()) != languages.end()) {
      YAML::Node seq = it->second;

/*
      if (!seq.IsSequence()) {
        throw std::exception("bad data...")
      }
*/

      std::cout << lang << it->first.as<std::string>() << def << std::endl;

      for (auto it_seq = seq.begin() ; it_seq != seq.end(); it_seq++) {
        std::cout << (*it_seq)["name"] << std::endl;
      }

    }
  }

}
