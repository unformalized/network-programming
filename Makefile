CC = clang
CFLAGS = -Wall -g -O2 -std=gnu99

BIN_CLIENT = ./bin/client
BIN_SERVER = ./bin/server

SRC_DIR = ./src

# common
COMMON = $(SRC_DIR)/common/error.c $(SRC_DIR)/common/io.c

# 
CLIENT = $(SRC_DIR)/client/chat_client.c
SERVER = $(SRC_DIR)/server/chat_server.c

THREAD_OPTIONS = -D_REENTRANT -lpthread

.PHONY:client
client:
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(COMMON) $(THREAD_OPTIONS) $(CLIENT) -o $(BIN_CLIENT)
#	$(BIN_CLIENT) 127.0.0.1 9190

.PHONY:server
server:
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(COMMON) $(THREAD_OPTIONS) $(SERVER) -o $(BIN_SERVER)
	$(BIN_SERVER) 9190


