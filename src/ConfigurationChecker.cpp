#include "ConfigurationChecker.hpp"

static const std::string HOME = "HOME";

const std::string default_scripts_dir = "Scripts";
const std::string default_config_dir = ".autobahn";
const std::string config_file = "config.yaml";
const std::string data_file = "data.yaml";

const std::vector<std::string> init_languages {"PERL", "RUBY", "LUA", "PYTHON2"};

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

  #ifdef LANGUAGES_DIR
    _languages_dir = path(LANGUAGES_DIR);
  #else
    throw InitializationException("unable to find source path");
  #endif
}

bool ConfigurationChecker::isInitialized() {
  path config_file_path = path(_config_dir).append('/' + config_file);
  return exists(config_file_path);
}

bool ConfigurationChecker::checkLanguage(const std::string& lang) const {
  std::vector<std::string> langs = loadLanguages();
  bool isCheckedLang = false;

  if (std::find(langs.begin(), langs.end(), lang) != langs.end()) {
      isCheckedLang = true;
  }

  return isCheckedLang;
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

  YAML::Node config_node;
  config_node["DIRECTORY"] = _scripts_dir.string();

  if (exists(_languages_dir) && is_directory(_languages_dir)) {
    auto directory_it = directory_iterator(_languages_dir);
    for (auto it = begin(directory_it); it != end(directory_it); it++) {
      if (is_regular_file(*it)) {
        std::cout << *it << std::endl;
      }
    }
  }

  for (const std::string& lang : init_languages) {
    config_node["LANGUAGES"].push_back(lang);
  }

  YAML::Emitter config_out;
  config_out << config_node;

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

YAML::Node ConfigurationChecker::loadConfig() const {
  path config_file_path = path(_config_dir).append('/' + config_file);
  return YAML::LoadFile(config_file_path.string());
}

const std::vector<std::string> ConfigurationChecker::loadLanguages() const {
  path config_file_path = path(_config_dir).append('/' + config_file);
  YAML::Node config_tree = YAML::LoadFile(config_file_path.string());

  YAML::Node lang_tree = config_tree["LANGUAGES"];
  std::vector<std::string> langs;

  if (lang_tree.IsSequence()) {
    for (auto it_seq = lang_tree.begin() ; it_seq != lang_tree.end(); it_seq++) {
      langs.push_back((*it_seq).as<std::string>());
    }
  }
  else {
    throw InitializationException("Languages are not initialized!");
  }

  return langs;
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
    if (std::find(init_languages.begin(), init_languages.end(), it->first.as<std::string>()) != init_languages.end()) {
      YAML::Node seq = it->second;

      std::cout << lang << it->first.as<std::string>() << def << std::endl;

      for (auto it_seq = seq.begin() ; it_seq != seq.end(); it_seq++) {
        std::cout << (*it_seq)["name"] << std::endl;
      }

    }
  }
}

YAML::Node ConfigurationChecker::isValidScript(const std::string& script_name) const {
  std::string script_lang;
  YAML::Node data_tree = loadData();
  YAML::Node script_info;
  bool isValid = false;

  for (auto it = data_tree.begin(); it != data_tree.end(); it++) {
    if ((it->second).IsSequence()) {

      script_lang = (it->first).as<std::string>();

      for (auto it_seq = (it->second).begin(); it_seq != (it->second).end(); it_seq++) {
        if (script_name.compare((*it_seq)["name"].as<std::string>()) == 0) {
          script_info["name"] = (*it_seq)["name"];
          script_info["lang"] = script_lang;
          script_info["path"] = (*it_seq)["path"];

          isValid = true;
          break;
        }
      }
    }
  }

  if (!isValid) {
    throw invalid_argument("invalid script name!");
  }
  return script_info;
}
