#!/usr/bin/lua 

function myPower(x)
    return function(y) return y^x end
end

power2 = myPower(2)
power3 = myPower(3)

print("sum ="..power2(5))
print("sum ="..power3(6))
