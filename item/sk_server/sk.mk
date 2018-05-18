HOME_DIR=./
INC_DIR=${HOME_DIR}include
LIB_DIR=$(HOME_DIR)/lib
LOC_DIR=./src
#JSON_DIR=${HOME_DIR}json 
JSON_DIR=./json/rapidjson

INCLUDE = -I./${INC_DIR}/ -I./$(JSON_DIR)

#LIB = -L$(LIB_DIR) -lrt -libmyframe -lpthread 
LIB = -L$(LIB_DIR)/ -lmyframe -lrt -lsign -lpthread \
	  /usr/lib/libcrypt.a \
	  /usr/lib/libcrypto.a \
	  /usr/lib/libssl.a \


CPPFLAGS = -gdwarf-2 -msse3 -W $(INCLUDE) -DDEBUG=1 -O0 -std=c++11
CC  = g++ $(CPPFLAGS)


OBJ	= $(LOC_DIR)/main.o     \
	  $(LOC_DIR)/real_single_dict.o \
	  $(LOC_DIR)/real_quotation_dict.o \
	  $(LOC_DIR)/ban_dict.o \
	  $(LOC_DIR)/finance_dict.o \
	  $(LOC_DIR)/id_dict.o \
	  $(LOC_DIR)/ua_dict.o \
	  $(LOC_DIR)/strategy_dict.o \
	  $(LOC_DIR)/sk_conf.o \
	  $(LOC_DIR)/proc_data.o \
	  $(LOC_DIR)/rquotation_data_process.o \
	  $(LOC_DIR)/rsingle_data_process.o \
	  $(LOC_DIR)/skhttp_res_data_process.o \
	  $(LOC_DIR)/address_dict.o \
	  $(LOC_DIR)/plate_dict.o \
	  $(LOC_DIR)/history_single_dict.o \
	  $(LOC_DIR)/history_quotation_dict.o \


AR=ar

server=sk_ser

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

