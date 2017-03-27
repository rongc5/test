HOME_DIR=../
INC_DIR=${HOME_DIR}core
IDL_DIR=$(HOME_DIR)idl
LIB_DIR=../lib

INCLUDE = -I./${INC_DIR}/ -I./${IDL_DIR}/

LIB = -L$(LIB_DIR) -lmyframe -lprotobuf -lpthread

CPPFLAGS = -g -msse3 -Wall $(INCLUDE) -DDEBUG=1
CC  = g++ $(CPPFLAGS)


OBJ	= $(IDL_DIR)/myframe.pb.o \
	./test_server.o     \
	./test_data_process.o     \
	./test_net_thread.o     \


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

$(client):client.o
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIB)


#=================================================
$(server):$(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIB)

%.o : %.cc *.h
	$(CC) $(CXXFLAGS) -c $<

%.o : %.cpp *.h
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB)

