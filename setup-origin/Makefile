.PHONY: help build test clean

# 빌드 아웃풋 디렉토리 설정
OUT_DIR := $(abspath $(CURDIR)/out)
SRC_DIR := $(CURDIR)/src
OBJ_DIR   := $(OUT_DIR)/obj
BIN_DIR   := $(OUT_DIR)/bin

help: ## Show available targets
	@grep -E '^[a-zA-Z0-9_%/-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

build: $(OUT_DIR) ## Build all executables
	$(MAKE) -C src OUT_DIR=$(OUT_DIR)

test: $(OUT_DIR) ## Run tests on rbtree implementation
	$(MAKE) -C test OUT_DIR=$(OUT_DIR) test

rebuild-test: clean $(OUT_DIR) ## Clean and rebuild test-rbtree for debugging
	$(MAKE) -C test OUT_DIR=$(OUT_DIR) all

visualize: $(OUT_DIR) ## Visualize RBTree -> check test/visualize-main.c
	$(MAKE) -C test OUT_DIR=$(OUT_DIR) SRC_DIR=$(SRC_DIR) visualize

clean: ## Clean build environment
	$(MAKE) -C src OUT_DIR=$(OUT_DIR) clean
	$(MAKE) -C test OUT_DIR=$(OUT_DIR) clean
	rm -rf $(OUT_DIR)

$(OUT_DIR):
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)
