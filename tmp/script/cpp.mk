CC = g++

CFLAGS = -c -Wall
CFLAGS += -O2

HEADS = $(wildcard ./*.h)

SOURCES = $(wildcard ./*.cpp)
OBJS = $(SOURCES:.cpp=.o)
TARGET = ./app.exe

all:$(TARGET)
$(TARGET):$(OBJS) $(SOURCES) $(HEADS)
	$(CC) -o $@ $(OBJS)

$(OBJS):%.o:%.cpp $(HEADS) 
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) ./$(TARGET)
