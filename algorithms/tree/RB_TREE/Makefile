CC = gcc
CFLAGS = -g -c -w
CFLAGS += -O2

HEADS = $(wildcard ./*.h)
SOURCES = $(wildcard ./*.c)
OBJS = $(SOURCES:.c=.o)
TARGET = app


$(TARGET):$(OBJS) $(SOURCES) $(HEADS)
	$(CC) -g -o $@ $(OBJS) -DTEST

$(OBJS):%.o:%.c $(HEADS) 
	$(CC) -g $(CFLAGS) $< -o $@ -DTEST

clean:
	rm -f $(OBJS) ./$(TARGET)

