--Hello World in Lua
print("Hello World.")
x = string.gsub("hello world", "%w+", "%0 %0", 1)
print(x)
