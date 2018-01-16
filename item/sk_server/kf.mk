HOME_DIR=./
INC_DIR=${HOME_DIR}include
LIB_DIR=$(HOME_DIR)/lib
LOC_DIR=./src
#JSON_DIR=${HOME_DIR}json 
JSON_DIR=./

INCLUDE = -I./${INC_DIR}/ -I./$(JSON_DIR)

#LIB = -L$(LIB_DIR) -lrt -libmyframe -lpthread 
LIB = -L$(LIB_DIR)/ -lmyframe -lrt -lpthread \
	  /usr/lib/libcrypt.a \
	  /usr/lib/libcrypto.a \
	  /usr/lib/libssl.a \


CPPFLAGS = -gdwarf-2 -msse3 -W $(INCLUDE) -DDEBUG=1 -O0
CC  = g++ $(CPPFLAGS)


OBJ	= $(LOC_DIR)/main.o     \
	  $(LOC_DIR)/sah_data_process.o \


AR=ar

server=sah_ser

all	: $(server)
	

Ctags	: 
	ctags *.h *.cpp

clean	:
	/bin/rm -f $(OBJ)
	rm -f $(server)
	rm -rf logs


#=================================================
$(server):$(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LIB)
	mkdir -p include
	cp $(LOC_DIR)/*.h include

%.o : %.cc *.h
	$(CC) $(CXXFLAGS) -c $<

%.o : %.cpp *.h
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB)

