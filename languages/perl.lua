local PERL = {}

function PERL:new ()
  local newObj = {lang = 'PERL', interpretor = 'perl'}
  self.__index = self
  return setmetatable(newObj, self)
end

function PERL:getInfo ()
  return self.lang
end

function PERL:execute (script_path)
  os.execute(self.interpretor .. ' ' .. script_path)
end

return perl
