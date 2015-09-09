#include "Execute.hpp"

Execute::Execute(const std::vector<std::string>& args, const ConfigurationChecker& conf):
_formula_manager(FormulaManager()),
Command(conf) {
  checkArguments(args);
  _script_name = args[1];
}

void Execute::checkArguments(const std::vector<std::string>& args) {
  if (args.size() != 2) {
    throw std::invalid_argument("incorrect 'exec' parameters.");
  }

  if (_conf.isValidScript(args[1])) {
    throw std::invalid_argument("this script name doesn't exist in database!");
  }
}

void Execute::run() {
  YAML::Node script_info = _conf.getScriptInfo(_script_name);
  std::string script_lang = script_info["lang"].as<std::string>();

  std::string lang_dir = _conf.getLangDirPath().c_str();

  try {
    _formula_manager.getLuaStateForLanguages(lang_dir, script_lang);
    _formula_manager.execute(script_info["path"].as<std::string>());
  }
  catch (LuaException& e) {
    ostringstream msg;
    msg << "LUA ERROR: " << e.what();

    throw std::logic_error(msg.str());
  }
}
