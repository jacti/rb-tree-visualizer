#!/usr/bin/env bash
set -euo pipefail

# 이 스크립트 파일이 있는 디렉터리
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# PRJ_ROOT 기본값: SCRIPT_DIR의 부모
DEFAULT_PRJ_ROOT="$(dirname "$SCRIPT_DIR")"

# 0) GDB 경로 탐색
GDB_PATH="$(which gdb 2>/dev/null || true)"
if [[ -z "$GDB_PATH" ]]; then
  echo "Error: gdb를 찾을 수 없습니다. PATH에 gdb가 있나요?" >&2
  exit 1
fi

# 1) Docker 프로젝트 여부
read -rp "Is this a Docker project? (y/n) [default: n]: " IS_DOCKER
IS_DOCKER=${IS_DOCKER:-n}
if [[ ! "$IS_DOCKER" =~ ^[YyNn]$ ]]; then
  echo "Please answer y or n."
  exit 1
fi

# 2) 프로젝트 루트 경로 입력
read -rp "Enter your project root path (relative or absolute) [default: $DEFAULT_PRJ_ROOT]: " PRJ_ROOT
if [[ -z "$PRJ_ROOT" ]]; then
  PRJ_ROOT="$DEFAULT_PRJ_ROOT"
  echo "No input provided. Using default: $PRJ_ROOT"
fi
PRJ_ROOT="$(cd "$PRJ_ROOT" && pwd)"

if [[ "$IS_DOCKER" =~ ^[Yy]$ ]]; then
  # 3) Docker 전용: rbtree_lab 폴더 이름
  read -rp "Enter rbtree lab folder name [default: rbtree_lab]: " RBTREE_DIR
  if [[ -z "$RBTREE_DIR" ]]; then
    RBTREE_DIR="rbtree_lab"
    echo "No input provided. Using default: $RBTREE_DIR"
  fi

  "$SCRIPT_DIR/setup-origin/origin-env-setup.sh" "$PRJ_ROOT" "$RBTREE_DIR" "$GDB_PATH"
else
  "$SCRIPT_DIR/setup-origin/origin-env-setup.sh" "$PRJ_ROOT" "$GDB_PATH"
fi
