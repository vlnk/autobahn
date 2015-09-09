local python = {}

function python:new ()
  local newObj = {lang = 'PYTHON', interpretor = 'python'}
  self.__index = self
  return setmetatable(newObj, self)
end

function python:getInfo ()
  return self.lang
end

function python:execute (script_path)
  os.execute(self.interpretor .. ' ' .. script_path)
end

return python
