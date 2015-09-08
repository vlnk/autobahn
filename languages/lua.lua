local lua = {}

function lua:new ()
  local newObj = {lang = 'LUA', interpretor = 'lua'}
  self.__index = self
  return setmetatable(newObj, self)
end

function lua:getInfo ()
  return self.lang
end

function lua:execute (script_path)
  os.execute(self.interpretor .. ' ' .. script_path)
end

return lua
