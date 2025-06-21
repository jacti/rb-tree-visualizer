#!/usr/bin/env bash
set -euo pipefail

# 이 스크립트의 위치를 기준으로 상대 경로 참조
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# 프로젝트 루트 기본값: 스크립트 위치의 부모 디렉터리
DEFAULT_PRJ_ROOT="$(dirname "$SCRIPT_DIR")"

# 1) Docker 프로젝트 여부
read -rp "Is this a Docker project? (y/n) [default: n]: " INPUT_DOCKER
if [[ -z "$INPUT_DOCKER" ]]; then
  IS_DOCKER="n"
  echo "No input provided. Using default: n"
else
  IS_DOCKER="$INPUT_DOCKER"
fi

# 2) 프로젝트 루트 경로
read -rp "Enter your project root path (relative or absolute) [default: $DEFAULT_PRJ_ROOT]: " INPUT_ROOT
if [[ -z "$INPUT_ROOT" ]]; then
  PRJ_ROOT="$DEFAULT_PRJ_ROOT"
  echo "No input provided. Using default: $PRJ_ROOT"
else
  PRJ_ROOT="$INPUT_ROOT"
fi
# 절대 경로로 변환
PRJ_ROOT="$(cd "$PRJ_ROOT" && pwd)"
\ nif [[ "$IS_DOCKER" =~ ^[Yy]$ ]]; then
  # 3) rbtree lab 폴더 이름 (Docker 프로젝트인 경우)
  read -rp "Enter rbtree lab folder name [default: rbtree_lab]: " INPUT_RBTREE
  if [[ -z "$INPUT_RBTREE" ]]; then
    RBTREE_DIR="rbtree_lab"
    echo "No input provided. Using default: rbtree_lab"
  else
    RBTREE_DIR="$INPUT_RBTREE"
  fi

  # 실행 권한 부여 & 실행
  chmod +x "$SCRIPT_DIR/setup-docker/setup.sh"
  "$SCRIPT_DIR/setup-docker/setup.sh" "$PRJ_ROOT" "$RBTREE_DIR"
else
  # 실행 권한 부여 & 실행
  chmod +x "$SCRIPT_DIR/setup-origin/setup.sh"
  "$SCRIPT_DIR/setup-origin/setup.sh" "$PRJ_ROOT"
fi
