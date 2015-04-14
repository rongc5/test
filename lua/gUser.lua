#!/usr/bin/lua 

function getUserInfo(id)
    print(id)
    return "hao123", 37, "zm@haierubic.com"
end

name, age, email, website = getUserInfo(1)

print("sum =", name, age, email, website)
