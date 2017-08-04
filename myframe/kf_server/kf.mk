HOME_DIR=../
INC_DIR=${HOME_DIR}core
LIB_DIR=$(HOME_DIR)/lib
LOC_DIR=./src
JSON_DIR=${HOME_DIR}json 

INCLUDE = -I./${INC_DIR}/ -I./$(JSON_DIR)

LIB = -L$(LIB_DIR) -lrt -libmyframe -lpthread \
	  /usr/lib/libcrypt.a \
	  /usr/lib/libcrypto.a \
	  /usr/lib/libssl.a \


CPPFLAGS = -gdwarf-2 -msse3 -W $(INCLUDE) -DDEBUG=1
CC  = g++ $(CPPFLAGS)


OBJ	= $(LOC_DIR)/main.o     \
	  $(LOC_DIR)/kf_data_process.o \


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
	mkdir -p include
	cp $(LOC_DIR)/*.h include

%.o : %.cc *.h
	$(CC) $(CXXFLAGS) -c $<

%.o : %.cpp *.h
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB)

