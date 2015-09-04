#include "Push.hpp"

void Pusher::push(const ConfigurationChecker& conf) {
  std::transform(_script_lang.begin(), _script_lang.end(), _script_lang.begin(), ::toupper);

  path from(_script_file);
  path to(conf.getScriptDirPath().string() + '/' + _script_name);

  if (exists(from)) {
    copy_file(from, to);

    YAML::Node meta = getMetaData(to);
    YAML::Node data = conf.loadData();

    data["SIZE"] = data["SIZE"].as<int>() + 1;
    data[_script_lang].push_back(meta);
    conf.saveData(data);
  }
  else {
    throw std::invalid_argument("Incorrect script file.");
  }
}

Pusher* Pusher::checkPushArguments(std::vector<std::string> args) {
  Pusher * pusher;

  if (args.size() == 4) {
    pusher = new Pusher(args[1], args[2], args[3]);
  }
  else {
    throw std::invalid_argument("Incorrect 'Push' parameters.");
  }

  return pusher;
}

YAML::Node Pusher::getMetaData(const path& location) {
  YAML::Node node;
  node["name"] = _script_name;
  node["path"] = location.string();
  return node;
}
