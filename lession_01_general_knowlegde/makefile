CUR_DIR := .
SRC_DIR := $(CUR_DIR)/src
INC_DIR := $(CUR_DIR)/inc
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin
LIB_DIR := $(CUR_DIR)/lib
LIB_SHARED_DIR := $(LIB_DIR)/shared
LIB_STATIC_DIR := $(LIB_DIR)/static

create_objs:
	gcc -c -fPIC $(SRC_DIR)/EarthHello.c -o $(OBJ_DIR)/EarthHello.o -I $(INC_DIR)
	gcc -c -fPIC $(SRC_DIR)/AlienHello.c -o $(OBJ_DIR)/AlienHello.o -I $(INC_DIR)
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I $(INC_DIR)

create_shared_libs:
	gcc -shared $(OBJ_DIR)/EarthHello.o $(OBJ_DIR)/AlienHello.o -o $(LIB_SHARED_DIR)/libhelloshared.so

create_static_libs:
	ar rcs $(LIB_STATIC_DIR)/libhello.a $(OBJ_DIR)/EarthHello.o $(OBJ_DIR)/AlienHello.o

all: create_objs create_shared_libs create_static_libs
	gcc $(OBJ_DIR)/main.o $(LIB_SHARED_DIR)/libhelloshared.so -o $(BIN_DIR)/shared_libs
	gcc $(OBJ_DIR)/main.o -L$(LIB_STATIC_DIR) -lhello -o $(BIN_DIR)/static_libs

clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(LIB_SHARED_DIR)/*.so
	rm -rf $(LIB_STATIC_DIR)/*.a
	rm -rf $(BIN_DIR)/shared_libs
	rm -rf $(BIN_DIR)/static_libs