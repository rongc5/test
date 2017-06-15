HOME_DIR=../../
INC_DIR=${HOME_DIR}core
IDL_DIR=$(HOME_DIR)idl
LIB_DIR=$(HOME_DIR)/lib
LOC_DIR=.

INCLUDE = -I./${INC_DIR}/ -I./${IDL_DIR}/

LIB = -L$(LIB_DIR) -lnewframe -lpthread -lssl\
	  -L/usr/local/lib -lrt \
	  /usr/local/lib/libevent.a  \
	  /usr/local/lib/libevent_core.a  \
	  /usr/local/lib/libevent_extra.a  \
	  /usr/local/lib/libevent_openssl.a  \
	  /usr/local/lib/libevent_pthreads.a \


CPPFLAGS = -gdwarf-2 -msse3 -Wall $(INCLUDE) -DDEBUG=1
CC  = g++ $(CPPFLAGS)


OBJ	= $(LOC_DIR)/main.o     \
	$(LOC_DIR)/ws_cli_connect.o     \
	$(LOC_DIR)/ws_cli_thread.o     \


AR=ar

server=ws_cli

all	: $(server)
	

Ctags	: 
	ctags *.h *.cpp

clean	:
	/bin/rm -f $(OBJ)
	rm -f $(server)
	rm -rf logs


#=================================================
$(server):$(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIB)

%.o : %.cc *.h
	$(CC) $(CXXFLAGS) -c $<

%.o : %.cpp *.h
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB)

