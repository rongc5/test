#!/usr/bin/lua 



zm = {name = "zhangming", age = 27, email = "zm@haierubic.com"}

--arr = {1, 2, 3, 4, 5, 6}
arr = {"string", 100, "hao123", function() print("coolshell.cn") end}

for k, v in pairs(zm) do
    print(k, v)
end

--print("sum =", arr[1], arr[3], arr[4])
