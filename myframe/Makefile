HOME_DIR=./
SRC_DIR=${HOME_DIR}src
IDL_DIR=$(HOME_DIR)idl
INC_DIR=${HOME_DIR}include
LIB_DIR=${HOME_DIR}lib

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
    ${SRC_DIR}/log_helper.o                        \
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
	/bin/rm -rf $(LIB_DIR)/lib$(LIBNAME).a  $(INC_DIR)/*.h $(OBJ)

#=================================================
$(LIBNAME):$(OBJ)
	$(AR) -ruv $(SRC_DIR)/lib$@.a $(OBJ)
	mkdir -p $(LIB_DIR) $(NC_DIR)
	cp $(SRC_DIR)/*.h $(INC_DIR)
	mv $(SRC_DIR)/lib$@.a $(LIB_DIR)

%.o : %.cc
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB) $(INCLUDE)

%.o : %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB) $(INCLUDE)

