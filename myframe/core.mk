HOME_DIR=./
SRC_DIR=${HOME_DIR}core

INCLUDE = -I./${SRC_DIR}/
LIB = -L/usr/lib/ -lpthread \

CPPFLAGS = -g -msse3 -Wall $(INCLUDE) -DDEBUG=1
CC  = g++ $(CPPFLAGS)

OBJ	= ${SRC_DIR}/base_net_container.o                        \
    ${SRC_DIR}/base_thread.o                        \
    ${SRC_DIR}/base_net_thread.o                        \
    ${SRC_DIR}/base_data_process.o                        \
    ${SRC_DIR}/channel_data_process.o                        \
    ${SRC_DIR}/listen_data_process.o                        \
    ${SRC_DIR}/common_def.o                        \
    ${SRC_DIR}/common_epoll.o                        \
    ${SRC_DIR}/log_process.o                        \
    ${SRC_DIR}/base_net_obj.o                        \
    ${SRC_DIR}/thread_helper.o                        \
    ${SRC_DIR}/base_connect.o                        \
    ${SRC_DIR}/listen_connect.o                        \
    ${SRC_DIR}/base_listen_thread.o                        \
    ${SRC_DIR}/common_obj_container.o                        \
    ${SRC_DIR}/channel_data_process.o                        \
    ${SRC_DIR}/common_util.o                        \
    ${SRC_DIR}/base_timer_process.o                        \
    ${SRC_DIR}/base_timer.o                        \
    ${SRC_DIR}/web_socket_data_process.o                        \
    ${SRC_DIR}/web_socket_msg.o                        \
    ${SRC_DIR}/web_socket_process.o                        \
    ${SRC_DIR}/web_socket_req_process.o                        \
    ${SRC_DIR}/web_socket_res_process.o                        \
    ${SRC_DIR}/http_base_data_process.o                        \
    ${SRC_DIR}/http_base_process.o                        \
    ${SRC_DIR}/http_res_process.o                        \
    ${SRC_DIR}/mybase64.o                        \
    ${SRC_DIR}/common_timer_process.o                        \

OBJ_INCLUDE=

AR=ar
LIBNAME=myframe


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

