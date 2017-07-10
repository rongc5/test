HOME_DIR=../
INC_DIR=${HOME_DIR}core
LIB_DIR=../lib

INCLUDE = -I./${INC_DIR}/ 

LIB = -L$(LIB_DIR) -lmyframe -lpthread

CPPFLAGS = -g -msse3 -Wall $(INCLUDE) -DDEBUG=1
CC  = g++ $(CPPFLAGS)


OBJ	= ./test_log.o     \


AR=ar

server=test_server
client=test_client

all	: $(server)
	

Ctags	: 
	ctags *.h *.cpp

clean	:
	/bin/rm -f *.o 
	/bin/rm -f *~~ 
	rm -f $(server) $(client);

$(client):client.o
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIB)


#=================================================
$(server):$(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIB)

%.o : %.cc *.h
	$(CC) $(CXXFLAGS) -c $<

%.o : %.cpp *.h
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB)

