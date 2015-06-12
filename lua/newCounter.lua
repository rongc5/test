#!/usr/bin/lua 


function newCounter()
    local i = 0
    return function()
        i = i + 1
        return i
    end
end

c1 = newCounter()
print("sum ="..c1())
print("sum ="..c1())
print("sum ="..c1())
