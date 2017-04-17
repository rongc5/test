HOME_DIR=./
SRC_DIR=${HOME_DIR}core
IDL_DIR=$(HOME_DIR)idl

INCLUDE = -I./${SRC_DIR}/ -I./${IDL_DIR} -I/usr/local/include 
LIB = -L/usr/lib/ -lpthread \
	  -lprotobuf \
	   -L/usr/local/lib -lrt \
		-libevent.a  \
		 -libevent_core.a  \
		 -lbevent_extra.a  \
		 -libevent_openssl.a  \
		 -libevent_pthreads.a 

CPPFLAGS = -g -msse3 -Wall $(INCLUDE) -DDEBUG=1
CC  = g++ $(CPPFLAGS)

OBJ	= ${SRC_DIR}/base_connect.o                     \
    ${SRC_DIR}/base_thread.o                        \
    ${SRC_DIR}/base_net_thread.o                        \
    ${SRC_DIR}/client_thread.o                        \
    ${SRC_DIR}/client_connect.o                        \
    ${SRC_DIR}/channel_connect.o                        \
    ${SRC_DIR}/listen_connect.o                        \
    ${SRC_DIR}/tcp_connect.o                        \
    ${SRC_DIR}/job_connect.o                       \
    ${SRC_DIR}/job_thread.o                       \
    ${SRC_DIR}/http_client_connect.o                       \
    ${SRC_DIR}/common_connect.o                       \
    ${SRC_DIR}/common_def.o                        \
    ${SRC_DIR}/log_thread.o                        \
    ${SRC_DIR}/listen_thread.o                        \
    ${SRC_DIR}/thread_helper.o                       \
    ${IDL_DIR}/idl.pb.o  

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
