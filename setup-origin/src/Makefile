CC = gcc
CFLAGS = -Wall -g

SRC_DIR = .
OUT_DIR ?= ../out
OBJ_DIR = $(OUT_DIR)/obj
BIN_DIR = $(OUT_DIR)/bin

OBJS = $(OBJ_DIR)/driver.o $(OBJ_DIR)/rbtree.o $(OBJ_DIR)/visualize_rbtree.o
TARGET = $(BIN_DIR)/driver

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

all: $(TARGET)

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
