# This is writed for programms in the permit server

CC = g++
CPPFLAGS = -O0 -g3 -DDEBUG -DUNW_LOCAL_ONLY -D_GNU_SOURCE
OBJLIB = libaddr.so

addr: main.cpp $(OBJLIB)
	$(CC) -rdynamic  -I./ $(CPPFLAGS) -L./ -laddr -lpthread -L/usr/local/lib -lunwind -o $@ main.cpp
$(OBJLIB):addr.cpp
	$(CC)  -rdynamic -I./ $(CPPFLAGS) -fpic -shared -lpthread -o $@ $<

.PHONY: clean
clean:
	rm -f addr $(OBJLIB)



