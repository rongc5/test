HOME_DIR=../
INC_DIR=${HOME_DIR}core
IDL_DIR=./idl
LIB_DIR=$(HOME_DIR)/lib
LOC_DIR=./src
JSON_DIR=${HOME_DIR}json 

INCLUDE = -I./${INC_DIR}/ -I./${IDL_DIR}/ -I./$(JSON_DIR)

LIB = -L$(LIB_DIR) -lrt -lnewframe -lpthread \
	  /usr/lib/libcrypt.a \
	  /usr/lib/libcrypto.a \
	  /usr/lib/libssl.a \
	  /usr/local/lib/libprotobuf.a  \
	  /usr/local/lib/libevent.a  \
	  /usr/local/lib/libevent_core.a  \
	  /usr/local/lib/libevent_extra.a  \
	  /usr/local/lib/libevent_openssl.a  \
	  /usr/local/lib/libevent_pthreads.a  


CPPFLAGS = -gdwarf-2 -msse3 -Wall $(INCLUDE) -DDEBUG=1
CC  = g++ $(CPPFLAGS)


OBJ	= $(LOC_DIR)/main.o     \
	$(LOC_DIR)/qfws_res_data_process.o     \
	$(LOC_DIR)/qfws_msg_process.o     \
	$(LOC_DIR)/http_server_thread.o     \
	$(LOC_DIR)/ws_ser_thread.o     \
	$(IDL_DIR)/ws_ser.pb.o     \


AR=ar

server=ws_ser

all	: $(server)
	

Ctags	: 
	ctags *.h *.cpp

clean	:
	/bin/rm -f $(OBJ)
	rm -f $(server)
	rm -rf logs
	rm -rf include/*


#=================================================
$(server):$(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIB)
	cp $(LOC_DIR)/*.h include

%.o : %.cc *.h
	$(CC) $(CXXFLAGS) -c $<

%.o : %.cpp *.h
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB)

