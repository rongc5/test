HOME_DIR=./
SRC_DIR=${HOME_DIR}core

# SSL支持配置 (通过环境变量或命令行设置)
ENABLE_SSL ?= 0

INCLUDE = -I./include/
LIB = -L/usr/lib64/libpthread.a -L./lib/ \

ifeq ($(ENABLE_SSL), 1)
    CPPFLAGS = -gdwarf-2 -Wall $(INCLUDE) -DDEBUG=1 -O0 -std=c++11 -DENABLE_SSL
    LIB += -lssl -lcrypto
else
    CPPFLAGS = -gdwarf-2 -Wall $(INCLUDE) -DDEBUG=1 -O0 -std=c++11
endif

CC  = g++ $(CPPFLAGS)

# 基础对象文件
OBJ_BASE = ${SRC_DIR}/common_obj_container.o                \
    ${SRC_DIR}/base_thread.o                        \
    ${SRC_DIR}/base_net_thread.o                        \
    ${SRC_DIR}/base_data_process.o                        \
    ${SRC_DIR}/channel_data_process.o                        \
    ${SRC_DIR}/common_def.o                        \
    ${SRC_DIR}/common_epoll.o                        \
    ${SRC_DIR}/base_net_obj.o                        \
    ${SRC_DIR}/common_util.o                        \
    ${SRC_DIR}/base_timer.o                        \
    ${SRC_DIR}/web_socket_data_process.o                        \
    ${SRC_DIR}/web_socket_msg.o                        \
    ${SRC_DIR}/web_socket_process.o                        \
    ${SRC_DIR}/web_socket_req_process.o                        \
    ${SRC_DIR}/web_socket_res_process.o                        \
    ${SRC_DIR}/http_base_data_process.o                        \
    ${SRC_DIR}/http_base_process.o                        \
    ${SRC_DIR}/http_res_process.o                        \
    ${SRC_DIR}/http_req_process.o                        \
    ${SRC_DIR}/mybase64.o                        \
    ${SRC_DIR}/common_domain.o                        \
    ${SRC_DIR}/common_cfgparser.o

# SSL对象文件 (仅在启用SSL时编译)
ifeq ($(ENABLE_SSL), 1)
    OBJ_SSL = ${SRC_DIR}/ssl_context.o
else
    OBJ_SSL =
endif

# 所有对象文件
OBJ = $(OBJ_BASE) $(OBJ_SSL)

OBJ_INCLUDE=

AR=ar
LIBNAME=myframe


all	: $(LIBNAME)

clean	:
	/bin/rm -rf  $(OBJ) $(SRC_DIR)/lib$(LIBNAME).a

#=================================================
$(LIBNAME):$(OBJ)
	$(AR) -ruv $(SRC_DIR)/lib$@.a $(OBJ)

%.o : %.cc
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB) $(INCLUDE)

%.o : %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB) $(INCLUDE)

