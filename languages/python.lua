local PYTHON = {}

function PYTHON:new ()
  local newObj = {lang = 'PYTHON', interpretor = 'python'}
  self.__index = self
  return setmetatable(newObj, self)
end

function PYTHON:getInfo ()
  return self.lang
end

function PYTHON:execute (script_path)
  os.execute(self.interpretor .. ' ' .. script_path)
end

return python
