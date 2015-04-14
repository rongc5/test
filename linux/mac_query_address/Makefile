CC = gcc

CFLAGS := -I ./ 
LIBS := -L/usr/lib/mysql/ -lmysqlclient
BIN = app 
OBJS = main.o cityidquery.o  ip_addr_info.o
#OBJ = $(addprefix obj/,$(OBJS))
LIBSO = libip_get_cityid.so.1.0.0




ALL:$(BIN) $(LIBSO)
$(LIBSO):cityidquery.o ip_addr_info.o
	$(CC) $(CFLAGS) -o $@ -shared -fpic $^ $(LIBS)
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) -o $@  $^ $(LIBS)
%.o:%.c
	$(CC) $(CFLAGS) -c -o  $@  $< $(LIBS)

BACK:main.c cityidquery.c cityidquery.h ip_addr_info.c Makefile haieru_config.conf qqwry.dat
	tar -czvf backup.tar.gz $^

.PHONY:clean
clean:
	-rm -rf $(OBJS) $(BIN) $(LIBSO)
