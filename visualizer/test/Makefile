.PHONY: all test visualize clean

CC = gcc
CFLAGS = -I ../src -Wall -g -DSENTINEL

SRC_DIR ?= ../src

OUT_DIR ?= ../out
BIN_DIR := $(OUT_DIR)/bin
OBJ_DIR := $(OUT_DIR)/obj

TARGET = $(BIN_DIR)/test-rbtree
OBJS = $(OBJ_DIR)/test-rbtree.o $(OBJ_DIR)/rbtree.o

# VISUALIZE 등록
VISUALIZE = $(BIN_DIR)/visualize_rbtree
VISUAL_OBJS = $(OBJ_DIR)/visualize-main.o $(OBJ_DIR)/rbtree_visualizer.o $(OBJ_DIR)/rbtree.o

# 1) 기본 빌드 타겟
all: test visualize

# --- build-only 타겟 ---
test: $(TARGET)
visualize: $(VISUALIZE)

# --- run-… 패턴룰 (build-only 타겟 의존 + 실행) ---
# $* 이 “test” 또는 “visualize” 로 치환됩니다.
EXEC_test      := $(notdir $(TARGET))
EXEC_visualize := $(notdir $(VISUALIZE))

run-%: % 
	@echo "→ Running $*"
	@cd $(OUT_DIR) && ./bin/$(EXEC_$*)

check-%: %
	@echo "→ Checking $* under Valgrind"
	@cd $(OUT_DIR) && \
		valgrind --leak-check=full --show-leak-kinds=all ./bin/$(EXEC_$*)

# 실행 파일 생성
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# visualize 실행 파일 생성
$(VISUALIZE): $(VISUAL_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# 1) rbtree.o 만 src에서 build
$(OBJ_DIR)/rbtree.o: $(SRC_DIR)/rbtree.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# 2) 그 외 .c → .o 변환을 단일 패턴룰로 처리
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(VISUAL_OBJS) $(OBJS) $(TARGET) $(VISUALIZE) $(OBJ_DIR)/rbtree.o
