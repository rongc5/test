CC = g++

INC_DIR = ./libiop/inc -I./inc

CFLAGS = -c -Wall
CFLAGS += -O2
CFLAGS += -pipe -fomit-frame-pointer -fpic -D_MP_MODE 
CFLAGS += -I$(INC_DIR)

LFLAGS= -L./bin -liop -lpthread

HEADS = $(wildcard $(INC_DIR)/*.h)

SOURCES = $(wildcard ./src/*.cpp)
OBJS = $(SOURCES:.cpp=.o)
TARGET = ./bin/imgs

all:$(TARGET)
$(TARGET):$(OBJS) $(SOURCES) $(HEADS)
	$(CC) -o $@ $(OBJS) $(LFLAGS)

$(OBJS):%.o:%.cpp $(HEADS) 
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) ./$(TARGET)
