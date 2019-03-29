HOME_DIR=./
INC_DIR=${HOME_DIR}include
LIB_DIR=$(HOME_DIR)/lib
LOC_DIR=./src
#JSON_DIR=${HOME_DIR}json 
JSON_DIR=./json/rapidjson

INCLUDE = -I./${INC_DIR}/ -I./$(JSON_DIR)

#LIB = -L$(LIB_DIR) -lrt -libmyframe -lpthread 
LIB = -L$(LIB_DIR)/ -lmyframe -lrt -lsign -lpthread \
	  /usr/lib64/libcrypt.a \
	  /usr/lib64/libcrypto.a \
	  /usr/lib64/libssl.a \


CPPFLAGS = -gdwarf-2 -msse3 -W $(INCLUDE) -DDEBUG=1 -O0 -std=c++11
CC  = g++ $(CPPFLAGS)


OBJ	= $(LOC_DIR)/main.o     \
	  $(LOC_DIR)/ban_dict.o \
	  $(LOC_DIR)/finance_dict.o \
	  $(LOC_DIR)/id_dict.o \
	  $(LOC_DIR)/ua_dict.o \
	  $(LOC_DIR)/sk_conf.o \
	  $(LOC_DIR)/proc_data.o \
	  $(LOC_DIR)/rquotation_data_process.o \
	  $(LOC_DIR)/rsingle_data_process.o \
	  $(LOC_DIR)/skhttp_res_data_process.o \
	  $(LOC_DIR)/addr_dict_split.o \
	  $(LOC_DIR)/addr_dict.o \
	  $(LOC_DIR)/plate_dict_split.o \
	  $(LOC_DIR)/plate_dict.o \
	  $(LOC_DIR)/history_single_dict.o \
	  $(LOC_DIR)/history_quotation_dict.o \
	  $(LOC_DIR)/holiday_dict.o \
	  $(LOC_DIR)/strategy_conf.o \
	  $(LOC_DIR)/reload_thread.o \
	  $(LOC_DIR)/skhttp_req_thread.o \
	  $(LOC_DIR)/sk_util.o \
	  $(LOC_DIR)/uhandler_select.o \
	  $(LOC_DIR)/uhandler_queryid.o \
	  $(LOC_DIR)/uhandler_default.o \
	  $(LOC_DIR)/address_search_index.o \
	  $(LOC_DIR)/change_rate_search_index.o \
	  $(LOC_DIR)/cir_value_search_index.o \
	  $(LOC_DIR)/down_pointer_search_index.o \
	  $(LOC_DIR)/end_avg_end10_search_index.o \
	  $(LOC_DIR)/end_avg_end20_search_index.o \
	  $(LOC_DIR)/end_avg_end30_search_index.o \
	  $(LOC_DIR)/end_avg_end5_search_index.o \
	  $(LOC_DIR)/end_avg_end_search_index.o \
	  $(LOC_DIR)/end_end10_search_index.o \
	  $(LOC_DIR)/end_end20_search_index.o \
	  $(LOC_DIR)/end_end30_search_index.o \
	  $(LOC_DIR)/end_end5_search_index.o \
	  $(LOC_DIR)/end_search_index.o \
	  $(LOC_DIR)/hqchange_rate_search_index.o \
	  $(LOC_DIR)/hqend_hqstart_search_index.o \
	  $(LOC_DIR)/hqrange_percent_search_index.o \
	  $(LOC_DIR)/hq_sum_change_rate_search_index.o \
	  $(LOC_DIR)/hq_sum_range_percent_search_index.o \
	  $(LOC_DIR)/hsingle_diff_search_index.o \
	  $(LOC_DIR)/hsingle_sum_diff_search_index.o \
	  $(LOC_DIR)/jlrgr_search_index.o \
	  $(LOC_DIR)/mgsy_search_index.o \
	  $(LOC_DIR)/mgxj_search_index.o \
	  $(LOC_DIR)/pb_search_index.o \
	  $(LOC_DIR)/pe_search_index.o \
	  $(LOC_DIR)/plate_search_index.o \
	  $(LOC_DIR)/range_percent_search_index.o \
	  $(LOC_DIR)/rsingle_diff2_search_index.o \
	  $(LOC_DIR)/rsingle_diff_search_index.o \
	  $(LOC_DIR)/up_pointer_search_index.o \
	  $(LOC_DIR)/value_search_index.o \
	  $(LOC_DIR)/zysrgr_search_index.o \
	  $(LOC_DIR)/block_search_index.o \
	  $(LOC_DIR)/hqdown_pointer_search_index.o \
	  $(LOC_DIR)/hqup_pointer_search_index.o \


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

