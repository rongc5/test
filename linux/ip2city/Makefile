CC = g++ -g

CFLAGS = -fPIC
INCLUDES =
LIBS = -L/usr/lib/mysql -lmysqlclient -lpthread
SRCS = ip_addr_info.c cityidquery.c main.c
OBJS = $(SRCS:.c=.o)
LIBSO = libip_get_cityid.so.1.0.0
BIN = app

all:$(BIN) $(LIBSO)
$(LIBSO):cityidquery.o ip_addr_info.o
	$(CC) -o $@ -shared $^ $(LIBS)
$(BIN):$(OBJS)
	$(CC) -g -o $@  $^ $(LIBS)
%.o:%.c
	$(CC) $(INCLUDES) $(CFLAGS) -c -o  $@  $<

pack:main.c cityidquery.c cityidquery.h ip_addr_info.c Makefile haieru_config.conf
	rm -f ip_get_cityid.tar.gz
	mkdir ip_get_cityid
	cp $^  ip_get_cityid
	tar -czvf ip_get_cityid.tar.gz ip_get_cityid
	rm -rf ip_get_cityid

.PHONY:clean
clean:
	-rm -rf $(OBJS) $(BIN) $(LIBSO)
