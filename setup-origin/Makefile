.PHONY: help build test clean

# 빌드 아웃풋 디렉토리 설정
BUILD_DIR := out
OBJ_DIR := $(abspath $(BUILD_DIR)/obj)
BIN_DIR := $(abspath $(BUILD_DIR)/bin)
SRC_DIR := $(CURDIR)/src

help: ## Show available targets
	@grep -E '^[a-zA-Z0-9_%/-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

build: $(BUILD_DIR) ## Build all executables
	$(MAKE) -C src OBJ_DIR=$(OBJ_DIR) BIN_DIR=$(BIN_DIR)

test: $(BUILD_DIR) ## Run tests on rbtree implementation
	$(MAKE) -C test OBJ_DIR=$(OBJ_DIR) BIN_DIR=$(BIN_DIR) test

rebuild-test: clean $(BUILD_DIR) ## Clean and rebuild test-rbtree for debugging
	$(MAKE) -C test OBJ_DIR=$(OBJ_DIR) BIN_DIR=$(BIN_DIR) all

visualize: clean $(BUILD_DIR)
	$(MAKE) -C test OBJ_DIR=$(OBJ_DIR) BIN_DIR=$(BIN_DIR) SRC_DIR=$(SRC_DIR) visualize

clean: ## Clean build environment
	$(MAKE) -C src OBJ_DIR=$(OBJ_DIR) BIN_DIR=$(BIN_DIR) clean
	$(MAKE) -C test OBJ_DIR=$(OBJ_DIR) BIN_DIR=$(BIN_DIR) clean
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(OBJ_DIR) $(BIN_DIR)
