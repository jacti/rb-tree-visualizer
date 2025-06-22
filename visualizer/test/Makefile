.PHONY: all test clean

CC = gcc
CFLAGS = -I ../src -Wall -g -DSENTINEL

SRC_DIR ?= ../src

# SRC_OBJ_DIR 설정
ifeq ($(origin OBJ_DIR),undefined)
    SRC_OBJ_DIR = $(SRC_DIR)
else
    SRC_OBJ_DIR = $(OBJ_DIR)
endif

BIN_DIR ?= $(CURDIR)
OBJ_DIR ?= $(CURDIR)

TARGET = $(BIN_DIR)/test-rbtree
OBJS = $(OBJ_DIR)/test-rbtree.o $(SRC_OBJ_DIR)/rbtree.o

# VISUALIZE 등록
VISUALIZE = $(BIN_DIR)/visualize_rbtree
VISUAL_OBJS = $(OBJ_DIR)/visualize-main.o $(OBJ_DIR)/rbtree_visualizer.o $(SRC_OBJ_DIR)/rbtree.o

all: $(TARGET) $(VISUALIZE)

visualize: $(VISUALIZE)
	# $(VISUALIZE)
	# valgrind $(visualize)

test: $(TARGET)
	$(TARGET)
	valgrind $(TARGET)

# 실행 파일 생성
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# visualize 실행 파일 생성
$(VISUALIZE): $(VISUAL_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# 1) rbtree.o 만 src/Makefile 호출로 빌드
$(OBJ_DIR)/rbtree.o:
	@$(MAKE) -C $(SRC_DIR) \
	  OBJ_DIR=$(SRC_OBJ_DIR) BIN_DIR=$(BIN_DIR) \
	  rbtree.o
	@mkdir -p $(@D)
	@cp $(SRC_OBJ_DIR)/rbtree.o $@

# 2) 그 외 .c → .o 변환을 단일 패턴룰로 처리
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@


# src 쪽 rbtree.o가 없다면 src/Makefile을 통해 빌드
$(OBJ_DIR)/rbtree.o:
	$(MAKE) -C $(SRC_DIR) OBJ_DIR=$(SRC_OBJ_DIR) BIN_DIR=$(BIN_DIR) $(SRC_OBJ_DIR)/rbtree.o

clean:
	rm -f $(OBJ_DIR)/test-rbtree.o $(TARGET) $(OBJ_DIR)/visualize_rbtree.o $(VISUALIZE) $(SRC_OBJ_DIR)/rbtree.o
