HOME_DIR=./
SRC_DIR=${HOME_DIR}core

INCLUDE = -I./${SRC_DIR}/ -I/usr/local/include 
LIB = 
#LIB =  -libevent.a  \
		 #-libevent_core.a  \
		 #-lbevent_extra.a  \
		 #-libevent_openssl.a  \
		 #-libevent_pthreads.a 

CPPFLAGS = -gdwarf-2 -msse3 -W $(INCLUDE) -DDEBUG=1
CC  = g++ $(CPPFLAGS)

OBJ	= ${SRC_DIR}/base_connect.o                     \
    ${SRC_DIR}/base_thread.o                        \
    ${SRC_DIR}/base_net_thread.o                        \
    ${SRC_DIR}/listen_connect.o                        \
    ${SRC_DIR}/tcp_connect.o                        \
    ${SRC_DIR}/http_client_connect.o                       \
    ${SRC_DIR}/http_client_thread.o                       \
    ${SRC_DIR}/base_data_process.o                       \
    ${SRC_DIR}/ws_data_process.o                       \
    ${SRC_DIR}/ws_req_data_process.o                      \
    ${SRC_DIR}/ws_res_data_process.o                       \
    ${SRC_DIR}/event_ws_msg.o                       \
    ${SRC_DIR}/common_def.o                        \
    ${SRC_DIR}/mybase64.o                        \
    ${SRC_DIR}/log_process.o                        \
    ${SRC_DIR}/listen_thread.o                        \
    ${SRC_DIR}/thread_helper.o                       \

OBJ_INCLUDE=

AR=ar
LIBNAME=newframe


all	: $(LIBNAME)

clean	:
	/bin/rm -rf  $(OBJ)

#=================================================
$(LIBNAME):$(OBJ)
	$(AR) -ruv $(SRC_DIR)/lib$@.a $(OBJ)

%.o : %.cc
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB) $(INCLUDE)

%.o : %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB) $(INCLUDE)
