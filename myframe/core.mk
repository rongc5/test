HOME_DIR=./
SRC_DIR=${HOME_DIR}core
BUILD_DIR=${HOME_DIR}build

# SSL支持配置 (通过环境变量或命令行设置)
ENABLE_SSL ?= 0

INCLUDE = -I./include/
LIB = -L/usr/lib64/libpthread.a -L./lib/ \

# 确保build目录存在
$(shell mkdir -p $(BUILD_DIR))

ifeq ($(ENABLE_SSL), 1)
    CPPFLAGS = -gdwarf-2 -Wall $(INCLUDE) -DDEBUG=1 -O0 -std=c++11 -DENABLE_SSL
    LIB += -lssl -lcrypto
else
    CPPFLAGS = -gdwarf-2 -Wall $(INCLUDE) -DDEBUG=1 -O0 -std=c++11
endif

CC  = g++ $(CPPFLAGS)

# 基础对象文件
OBJ_BASE = ${BUILD_DIR}/common_obj_container.o                \
    ${BUILD_DIR}/base_thread.o                        \
    ${BUILD_DIR}/base_net_thread.o                        \
    ${BUILD_DIR}/base_data_process.o                        \
    ${BUILD_DIR}/channel_data_process.o                        \
    ${BUILD_DIR}/common_def.o                        \
    ${BUILD_DIR}/common_epoll.o                        \
    ${BUILD_DIR}/base_net_obj.o                        \
    ${BUILD_DIR}/common_util.o                        \
    ${BUILD_DIR}/base_timer.o                        \
    ${BUILD_DIR}/web_socket_data_process.o                        \
    ${BUILD_DIR}/web_socket_msg.o                        \
    ${BUILD_DIR}/web_socket_process.o                        \
    ${BUILD_DIR}/web_socket_req_process.o                        \
    ${BUILD_DIR}/web_socket_res_process.o                        \
    ${BUILD_DIR}/http_base_data_process.o                        \
    ${BUILD_DIR}/http_base_process.o                        \
    ${BUILD_DIR}/http_res_process.o                        \
    ${BUILD_DIR}/http_req_process.o                        \
    ${BUILD_DIR}/mybase64.o                        \
    ${BUILD_DIR}/common_domain.o                        \
    ${BUILD_DIR}/common_cfgparser.o

# SSL对象文件 (仅在启用SSL时编译)
ifeq ($(ENABLE_SSL), 1)
    OBJ_SSL = ${BUILD_DIR}/ssl_context.o
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
	/bin/rm -rf  $(BUILD_DIR) $(SRC_DIR)/lib$(LIBNAME).a

#=================================================
$(LIBNAME):$(OBJ)
	$(AR) -ruv $(SRC_DIR)/lib$@.a $(OBJ)

# 编译规则：从源目录编译到build目录
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CC) -c $< -o $@ $(INCLUDE)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cc
	$(CC) -c $< -o $@ $(INCLUDE)

%.o : %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB) $(INCLUDE)

