HOME_DIR=./
SRC_DIR=${HOME_DIR}/core

INCLUDE = -I./${SRC_DIR}/
LIB = -L/usr/lib/ -lpthread \

CPPFLAGS = -gdwarf-2 -msse3 $(INCLUDE) -DDEBUG=1 -O0 -ggdb -DINFO -W -Wall -Werror
CC  = g++ $(CPPFLAGS) -std=c++11

#OBJ	= ${SRC_DIR}/sign.o                        
OBJ	= ${SRC_DIR}/ul_prime.o                        \
    ${SRC_DIR}/ul_sign.o                        \

OBJ_INCLUDE=

AR=ar
LIBNAME=sign


all	: $(LIBNAME)

clean	:
	/bin/rm -rf  $(OBJ) lib$(LIBNAME).a

#=================================================
$(LIBNAME):$(OBJ)
	$(AR) -ruv $(SRC_DIR)/lib$@.a $(OBJ)

%.o : %.cc
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB) $(INCLUDE)

%.o : %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@ $(LIB) $(INCLUDE)

