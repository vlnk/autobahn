#include "Rename.hpp"

Rename::Rename(const std::vector<std::string>& args, const ConfigurationChecker& conf):
_conf(conf) {
  checkArguments(args);
}

void Rename::checkArguments(const std::vector<std::string>& args) {
  /** 1) Check the old script name
   *  2) Check the new script name (already exist?)
   *  3) Change data.yaml
   *  4) Change path
   */

   if (args.size() != 3) {
     throw invalid_argument("command 'rename' has invalid arguments!");
   }

   fs::path script_dir = _conf.getScriptDirPath();

   std::string old_script_name = args[1];
   std::string new_script_name = args[2];

   bool isOldExist = _conf.isValidScript(old_script_name);
   bool isNewExist = _conf.isValidScript(new_script_name);

   fs::path new_script_path = script_dir.append('/' + new_script_name);
   if (fs::exists(new_script_path)) {
     throw invalid_argument("new script file already exists!");
   }

   if (isOldExist && !isNewExist) {
     _old_script_name = old_script_name;
     _new_script_name = new_script_name;
   }
   else if (!isOldExist) {
     throw invalid_argument("old script name doesn't exist in database!");
   }
   else if (isNewExist) {
     throw invalid_argument("new script name already exists in database!");
   }
}

void Rename::run() {
  YAML::Node data_tree = _conf.loadData();
  fs::path old_script_path = _conf.getScriptDirPath();
  fs::path new_script_path = _conf.getScriptDirPath();

  old_script_path.append('/' + _old_script_name);
  new_script_path.append('/' + _new_script_name);

  for (auto it = data_tree.begin(); it != data_tree.end(); it++) {
    if ((it->second).IsSequence()) {
      for (auto it_seq = (it->second).begin(); it_seq != (it->second).end(); it_seq++) {
        if (_old_script_name.compare((*it_seq)["name"].as<std::string>()) == 0) {
          (*it_seq)["name"] = _new_script_name;
          (*it_seq)["path"] = new_script_path.string();
          break;
        }
      }
    }
  }

  fs::copy(old_script_path, new_script_path);
  fs::remove(old_script_path);
  _conf.updateData(data_tree);
}
