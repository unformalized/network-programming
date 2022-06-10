CC = gcc-9
CFLAGS = -Wall -g -O2 -Werror -std=gnu99

BIN_CLIENT = ./bin/clinet
BIN_SERVER = ./bin/server
BIN_TCP_CLIENT = ./bin/tcp_clinet
BIN_TCP_SERVER = ./bin/tcp_server

SRC_DIR = ./src

# common
COMMON = $(SRC_DIR)/common/error.c

# 
CLINET = $(SRC_DIR)/client.c
SERVER = $(SRC_DIR)/server.c
TCP_CLIENT = $(SRC_DIR)/tcp_client.c
TCP_SERVER = $(SRC_DIR)/tcp_server.c

.PHONY:client
client:
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(COMMON) $(CLIENT) -o $(BIN_CLIENT)

.PHONY:server
server:
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(COMMON) $(SERVER) -o $(BIN_SERVER)

.PHONY:tcp_client
tcp_client:
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(COMMON) $(TCP_CLIENT) -o $(BIN_TCP_CLIENT)
	$(BIN_TCP_CLIENT) 127.0.0.1 9190

.PHONY:tcp_server
tcp_server:
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(COMMON) $(TCP_SERVER) -o $(BIN_TCP_SERVER)
	$(BIN_TCP_SERVER) 9190


clean:
	rm -f $(BIN_CLIENT) $(BIN_SERVER)


