CC = gcc

CFLAGS := -I ./include

BIN = bin/app.exe
OBJS = main.o localIp.o qqwry.o 
OBJ = $(addprefix obj/,$(OBJS))
$(BIN):$(OBJ)
	$(CC) $(CFLAGS) -o $@  $^ 
obj/%.o:src/%.c
	$(CC) $(CFLAGS) -c -o  $@  $<


.PHONY:clean
clean:
	-rm -rf $(OBJ) $(BIN)
