HOME_DIR=../../
INC_DIR=${HOME_DIR}core
IDL_DIR=$(HOME_DIR)idl
LIB_DIR=$(HOME_DIR)/lib

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


OBJ	= ./http_server.o     \
	 ./http_server_thread.o     \


AR=ar

hserver=http_server

all	: $(hserver)
	

Ctags	: 
	ctags *.h *.cpp

clean	:
	/bin/rm -f *.o 
	/bin/rm -f *~~ 
	rm -f $(server) $(client) $(hserver)
	rm -rf logs

$(hserver):$(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIB)

