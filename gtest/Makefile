CC = gcc
CPP = g++
LINK = g++
CFLAGS = -g -Wall -Werror -Wextra -std=gnu99
CPPFLAGS = -g -Wall -Werror -Wextra
LIBS =  -L/usr/lib/gtest -lgtest -lgtest_main


C__SOURCES = $(wildcard *.c)
#CPPSOURCES = $(wildcard *.cpp)
CPPSOURCES = add.cpp
OBJECTS = $(patsubst %.c, %.o, $(C__SOURCES)) $(patsubst %.cpp, %.o, $(CPPSOURCES))
TARGET = test


first: all


%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<


%.o: %.cpp
	$(CPP) -c $(CPPFLAGS) -o $@ $<


all: $(TARGET)


$(TARGET): $(OBJECTS)
	$(LINK) $(CPPFLAGS) $(LIBS) -o $(TARGET) $(OBJECTS)


.PHONY: clean


clean:
	rm -f $(TARGET) $(OBJECTS)

