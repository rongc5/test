HOME_DIR=../
LIB_DIR=${HOME_DIR}src/

INCLUDE = -I./${LIB_DIR}/ 		    

LIB = -lpthread

CPPFLAGS = -g -msse3 -Wall $(INCLUDE) -DDEBUG=1
CC  = g++ $(CPPFLAGS)

OUTPUT_PATH=./output

OBJ	= ${LIB_DIR}/base_net_container.o                        \
    ${LIB_DIR}/base_thread.o                        \
    ${LIB_DIR}/common_def.o                        \
    ${LIB_DIR}/channel_msg_process.o                        \
    ${LIB_DIR}/common_epoll.o                        \
    ${LIB_DIR}/common_thread.o                        \
    ${LIB_DIR}/log_helper.o                        \
    ${LIB_DIR}/net_obj.o                        \
    ${LIB_DIR}/passing_msg_thread.o                        \
    ${LIB_DIR}/thread_helper.o                        \
    ${LIB_DIR}/listen_data_process.o                        \
    ./test_server.o                        

EXEC=server

OBJ_INCLUDE=

AR=ar

all	: $(OBJ) $(EXEC)

Ctags	: 
	ctags *.h *.cpp

clean	:
	/bin/rm -f *.o 
	/bin/rm -f *~~ 
	rm -f $(EXEC) $(OBJ);
	rm -rf $(OUTPUT_PATH);

output	:
	mkdir -p $(OUTPUT_PATH)/include 
	cp -f $(OBJ_INCLUDE) $(OUTPUT_PATH)/include 
	mkdir -p $(OUTPUT_PATH)/bin
	cp -f $(EXEC) $(OUTPUT_PATH)/bin


#=================================================
$(EXEC):$(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIB)

%.o : %.cc *.h
	$(CC) $(CXXFLAGS) -c $<

%.o : %.cpp *.h
	$(CC) $(CXXFLAGS) -c $< -o $@

