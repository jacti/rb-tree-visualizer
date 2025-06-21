#!/usr/bin/env bash
set -euo pipefail

# 사용법 안내
echo "Usage: $0 <project_root_dir> <rbtree_lab_folder>"
if [[ $# -ne 2 ]]; then
  echo "Error: 프로젝트 루트 디렉터리와 rbtree_lab 폴더 이름을 인자로 전달해야 합니다." >&2
  echo "예: $0 /path/to/project_root rbtree_lab" >&2
  exit 1
fi

# 인자
PRJ_ROOT_DIR="$1"
RBTREE_DIR="$2"

# -- 플러그인 루트 기준 경로 계산 --
# 이 스크립트 파일이 있는 디렉터리
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# 플러그인 전체 루트(한 단계 위)
PLUGIN_ROOT="$(dirname "$SCRIPT_DIR")"

# 소스 디렉터리
# 1) VS Code 설정은 setup-docker/.vscode 에 있습니다.
VSCODE_SRC="$SCRIPT_DIR/.vscode"
# 2) 테스트용 파일은 통합된 visualizer/test 에 있습니다.
TEST_SRC="$PLUGIN_ROOT/visualizer/test"

# 대상 디렉터리 (프로젝트 루트 기준)
VSCODE_DST="$PRJ_ROOT_DIR/.vscode"
TEST_DST="$PRJ_ROOT_DIR/$RBTREE_DIR/test"

# 경로 유효성 검사
for dir in "$VSCODE_SRC" "$TEST_SRC" "$VSCODE_DST" "$TEST_DST"; do
  if [[ ! -e "$dir" ]]; then
    echo "Error: 경로를 찾을 수 없습니다: $dir" >&2
    exit 1
  fi
done

# .vscode 설정 덮어쓰기
echo "→ Updating $VSCODE_DST/launch.json and $VSCODE_DST/tasks.json"
cp -f "$VSCODE_SRC/launch.json" "$VSCODE_DST/launch.json"
cp -f "$VSCODE_SRC/tasks.json"  "$VSCODE_DST/tasks.json"

# rbtree_lab/test 파일 교체
echo "→ Updating $TEST_DST/Makefile and visualize-rbtree.c"
cp -f "$TEST_SRC/Makefile"           "$TEST_DST/Makefile"
cp -f "$TEST_SRC/visualize-rbtree.c" "$TEST_DST/visualize-rbtree.c"

echo "✔ setup-docker complete."
