HOME_DIR=./
SRC_DIR=${HOME_DIR}core
IDL_DIR=$(HOME_DIR)idl

INCLUDE = -I./${SRC_DIR}/ -I./${IDL_DIR}/
LIB = -L/usr/lib/ -lpthread \
	  -lprotobuf

CPPFLAGS = -g -msse3 -Wall $(INCLUDE) -DDEBUG=1
CC  = g++ $(CPPFLAGS)

OBJ	= ${SRC_DIR}/base_net_container.o                        \
    ${SRC_DIR}/base_thread.o                        \
    ${SRC_DIR}/common_def.o                        \
    ${SRC_DIR}/channel_msg_process.o                        \
    ${SRC_DIR}/common_epoll.o                        \
    ${SRC_DIR}/common_thread.o                        \
    ${SRC_DIR}/log_thread.o                        \
    ${SRC_DIR}/net_obj.o                        \
    ${SRC_DIR}/passing_msg_thread.o                        \
    ${SRC_DIR}/thread_helper.o                        \
    ${SRC_DIR}/listen_data_process.o                        \
    ${IDL_DIR}/myframe.pb.o                        

OBJ_INCLUDE=

AR=ar
LIBNAME=myframe


all	: $(LIBNAME)

clean	:
	/bin/rm -rf  $(OBJ)

#=================================================
$(LIBNAME):$(OBJ)
	$(AR) -ruv $(SRC_DIR)/lib$@.a $(OBJ)
	mv core/lib$@.a .

%.o : %.cc
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB) $(INCLUDE)

%.o : %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB) $(INCLUDE)

