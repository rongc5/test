HOME_DIR=../
INC_DIR=${HOME_DIR}core
LIB_DIR=../lib

INCLUDE = -I./${INC_DIR}/ 

LIB = -L$(LIB_DIR) -lsign -lpthread

CPPFLAGS = -g -msse3 -Wall $(INCLUDE) -DDEBUG=1
CC  = g++ $(CPPFLAGS) -std=c++11


OBJ	= ./sign.o     \

AR=ar

server=app

all	: $(server)
	

Ctags	: 
	ctags *.h *.cpp

clean	:
	/bin/rm -f *.o 
	/bin/rm -f *~
	rm -f $(server) $(client);


#=================================================
$(server):$(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIB)

%.o : %.cc *.h
	$(CC) $(CXXFLAGS) -c $<

%.o : %.cpp *.h
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB)

