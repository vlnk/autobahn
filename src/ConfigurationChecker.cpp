#include "ConfigurationChecker.hpp"

ConfigurationChecker::ConfigurationChecker() {
  #ifdef CONFIG_DIR
    _config_dir = fs::path(CONFIG_DIR);
  #else
    char const* tmp_config = getenv(HOME.c_str());
    if(tmp_config != NULL) {
      std::string home = std::string(tmp_config);
      _config_dir = fs::path(home + '/' + _default_config_dir);
    }
  #endif

  #ifdef SCRIPTS_DIR
    _scripts_dir = fs::path(SCRIPTS_DIR);
  #else
    char const* tmp_scripts = getenv(HOME.c_str());
    if(tmp_scripts != NULL) {
      std::string home = std::string(tmp_scripts);
      _scripts_dir = fs::path(home + '/' + _default_scripts_dir);
    }
  #endif

  #ifdef LANGUAGES_DIR
    _languages_dir = fs::path(LANGUAGES_DIR);
  #else
    throw InitializationException("unable to find source path");
  #endif
}

bool ConfigurationChecker::isInitialized() {
  fs::path config_file_path = fs::path(_config_dir).append('/' + _config_file);
  return fs::exists(config_file_path);
}

bool ConfigurationChecker::isValidLanguage(const std::string& lang) const {
  const std::vector<const std::string> langs = getLanguages();
  bool isValid = false;

  if (std::find(langs.begin(), langs.end(), lang) != langs.end())
    isValid = true;

  return isValid;
}

bool ConfigurationChecker::isValidPath(const fs::path& script_path) const {
  bool isValid = false;

  if (fs::exists(script_path) && fs::is_regular_file(script_path))
    isValid = true;

  return isValid;
}

bool ConfigurationChecker::isValidScript(const std::string& script_name) const {
  std::string script_lang;
  YAML::Node data_tree = loadData();
  bool isValid = false;

  for (auto it = data_tree.begin(); it != data_tree.end(); it++) {
    if ((it->second).IsSequence()) {

      script_lang = (it->first).as<std::string>();

      for (auto it_seq = (it->second).begin(); it_seq != (it->second).end(); it_seq++) {
        if ((script_name.compare((*it_seq)["name"].as<std::string>()) == 0) &&
            (isValidPath(fs::path((*it_seq)["path"].as<std::string>()))) &&
            (isValidLanguage(script_lang))){
          isValid = true;
          break;
        }
      }
    }
  }

  return isValid;
}

const std::vector<const std::string> ConfigurationChecker::getLanguages() const {
  std::vector<const std::string> languages;

  if (fs::exists(_languages_dir) && fs::is_directory(_languages_dir)) {
    auto directory_it = fs::directory_iterator(_languages_dir);
    for (auto it = begin(directory_it); it != end(directory_it); it++) {
      if (fs::is_regular_file(*it)) {
        fs::path file_name = fs::path(*it);
        languages.push_back(file_name.stem().string());
      }
    }
  }

  return languages;
}

const YAML::Node ConfigurationChecker::getScriptInfo(const std::string& script_name) const {
  YAML::Node script_info;

  if (isValidScript(script_name)) {
    YAML::Node data_tree = loadData();
    std::string script_lang;

    for (auto it = data_tree.begin(); it != data_tree.end(); it++) {
      if ((it->second).IsSequence()) {

        script_lang = (it->first).as<std::string>();

        for (auto it_seq = (it->second).begin(); it_seq != (it->second).end(); it_seq++) {
          if (script_name.compare((*it_seq)["name"].as<std::string>()) == 0) {
            script_info["name"] = (*it_seq)["name"];
            script_info["lang"] = script_lang;
            script_info["path"] = (*it_seq)["path"];
            break;
          }
        }
      }
    }
  }
  else {
    throw invalid_argument("unable to find requested script:" + script_name);
  }

  return script_info;
}

void ConfigurationChecker::initializeConfiguration(std::vector<std::string> args) {
  if (args.size() == 3) {
    if (fs::exists(args[1]) && fs::exists(args[2])) {
      _config_dir = fs::path(args[1]);
      _scripts_dir = fs::path(args[2]);
    }
    else {
      throw InitializationException("incorects directories!");
    }
  }

  fs::path config_file_path = fs::path(_config_dir).append('/' + _config_file);
  fs::path data_file_path = fs::path(_config_dir).append('/' + _data_file);

  std::ofstream config_file, data_file;

  fs::create_directory(_config_dir);
  fs::create_directory(_scripts_dir);

  config_file.open(config_file_path.string());

  YAML::Node config_node;
  config_node["DIRECTORY"] = _scripts_dir.string();

  const std::vector<const std::string> langs = getLanguages();
  for (const std::string& lang : langs) {
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

void ConfigurationChecker::appendData(const YAML::Node& node) const {
  fs::path data_file_path = fs::path(_config_dir).append('/' + _data_file);
  std::ofstream data_file;

  data_file.open(data_file_path.string());
  YAML::Emitter data_out;
  data_out << node;

  data_file << data_out.c_str();
  data_file.close();
}

void ConfigurationChecker::updateData(const YAML::Node& node) const {
  fs::path data_file_path = fs::path(_config_dir).append('/' + _data_file);
  std::ofstream data_file;

  data_file.open(data_file_path.string());
  data_file.clear();
  
  YAML::Emitter data_out;
  data_out << node;

  data_file << data_out.c_str();
  data_file.close();
}

void ConfigurationChecker::clean() {
  fs::remove_all(_config_dir);
  fs::remove_all(_scripts_dir);
}

void ConfigurationChecker::list() {
  Color::Painter def(Color::FG_DEFAULT);
  Color::Painter lang(Color::FG_YELLOW);
  YAML::Node data = loadData();

  const std::vector<const std::string> languages = getLanguages();

  for (auto it = data.begin(); it != data.end(); it++) {
    if (std::find(languages.begin(), languages.end(), it->first.as<std::string>()) != languages.end()) {
      YAML::Node seq = it->second;

      std::cout << lang << it->first.as<std::string>() << def << std::endl;

      for (auto it_seq = seq.begin() ; it_seq != seq.end(); it_seq++) {
        std::cout << (*it_seq)["name"] << std::endl;
      }
    }
  }
}
