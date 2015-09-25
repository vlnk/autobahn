local LUA = {}

function LUA:new ()
  local newObj = {lang = 'LUA', interpretor = 'lua'}
  self.__index = self
  return setmetatable(newObj, self)
end

function LUA:getInfo ()
  return self.lang
end

function LUA:execute (script_path)
  os.execute(self.interpretor .. ' ' .. script_path)
end

return lua
