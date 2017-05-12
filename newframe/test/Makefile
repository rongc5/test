HOME_DIR=../
INC_DIR=${HOME_DIR}core
IDL_DIR=$(HOME_DIR)idl
LIB_DIR=../lib

INCLUDE = -I./${INC_DIR}/ -I./${IDL_DIR}/

LIB = -L$(LIB_DIR) -lnewframe -lprotobuf -lpthread \
	  -L/usr/local/lib -lrt \
	  /usr/local/lib/libevent.a  \
	  /usr/local/lib/libevent_core.a  \
	  /usr/local/lib/libevent_extra.a  \
	  /usr/local/lib/libevent_openssl.a  \
	  /usr/local/lib/libevent_pthreads.a 


CPPFLAGS = -gdwarf-2 -msse3 -Wall $(INCLUDE) -DDEBUG=1
CC  = g++ $(CPPFLAGS)


OBJ	= $(IDL_DIR)/idl.pb.o \
	./test_server.o     \


AR=ar

server=test_server
client=test_client

all	: $(server) $(client)
	

Ctags	: 
	ctags *.h *.cpp

clean	:
	/bin/rm -f *.o 
	/bin/rm -f *~~ 
	rm -f $(server) $(client);


$(client):test_client.o $(IDL_DIR)/idl.pb.o
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIB)

#=================================================
$(server):$(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIB)

%.o : %.cc *.h
	$(CC) $(CXXFLAGS) -c $<

%.o : %.cpp *.h
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB)

