CC = gcc

CFLAGS := -I ./include
LIBS := -L/usr/lib/mysql/ -lmysqlclient
BIN = bin/app.exe 
OBJS = main.o cityidquery.o  ip_addr_info.o
OBJ = $(addprefix obj/,$(OBJS))
LIBSO = lib/libip_get_cityid.so.1.0.0




ALL:$(BIN) $(LIBSO)
$(LIBSO):obj/cityidquery.o obj/ip_addr_info.o
	$(CC) $(CFLAGS) -o $@ -shared -fpic $^ $(LIBS)
$(BIN):$(OBJ)
	$(CC) $(CFLAGS) -o $@  $^ $(LIBS)
obj/%.o:src/%.c
	$(CC) $(CFLAGS) -c -o  $@  $< $(LIBS)


.PHONY:clean
clean:
	-rm -rf $(OBJ) $(BIN) $(LIBSO)
