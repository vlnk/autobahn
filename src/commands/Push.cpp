#include "Push.hpp"

Push::Push(const std::vector<std::string>& args, const ConfigurationChecker& conf):
  _conf(conf) {
  checkArguments(args);
}

void Push::run() {
  std::transform(_script_lang.begin(), _script_lang.end(), _script_lang.begin(), ::tolower);

  fs::path from(_script_file);
  fs::path to(_conf.getScriptDirPath().string() + '/' + _script_name);

  if (!_conf.isValidLanguage(_script_lang)) {
    throw std::invalid_argument("Incorrect script language.");
  }

  if (fs::exists(from)) {
    copy_file(from, to);

    YAML::Node meta = getMetaData(to);
    YAML::Node data = _conf.loadData();

    data["SIZE"] = data["SIZE"].as<int>() + 1;
    data[_script_lang].push_back(meta);
    _conf.appendData(data);
  }
  else {
    throw std::invalid_argument("Incorrect script file.");
  }
}

void Push::checkArguments(const std::vector<std::string>& args) {
  if (args.size() == 4) {
    _script_file = args[1];
    _script_name = args[2];
    _script_lang = args[3];
  }
  else {
    throw std::invalid_argument("incorrect 'push' parameters.");
  }
}

YAML::Node Push::getMetaData(const fs::path& location) {
  YAML::Node node;
  node["name"] = _script_name;
  node["path"] = location.string();
  return node;
}
