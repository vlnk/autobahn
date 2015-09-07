local perl = {}

function perl:new ()
  local newObj = {lang = 'PERL', interpretor = 'perl'}
  self.__index = self
  return setmetatable(newObj, self)
end

function perl:getInfo ()
  print(self.lang)
end

function perl:execute (script_path)
  os.execute(self.interpretor .. ' ' .. script_path)
end

return perl
