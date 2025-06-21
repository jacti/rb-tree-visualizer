#!/usr/bin/env bash
set -euo pipefail

echo "Usage: $0 <project_root_dir> <rbtree_lab_folder> <gdb_path>"
if [[ $# -ne 3 ]]; then
  echo "Error: 프로젝트 루트 디렉터리, rbtree_lab 폴더, gdb 경로 순으로 인자를 전달해야 합니다." >&2
  echo "예: $0 /path/to/project_root rbtree_lab /usr/bin/gdb" >&2
  exit 1
fi

PRJ_ROOT_DIR="$1"
RBTREE_DIR="$2"
GDB_PATH="$3"

# 스크립트 & 플러그인 루트 산출
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PLUGIN_ROOT="$(dirname "$SCRIPT_DIR")"

# 소스 디렉터리
VSCODE_SRC="$SCRIPT_DIR/vscode"
TEST_SRC="$PLUGIN_ROOT/visualizer/test"

# 대상
VSCODE_DST="$PRJ_ROOT_DIR/.vscode"
TEST_DST="$PRJ_ROOT_DIR/$RBTREE_DIR/test"

# 경로 유효성 검사
for dir in "$VSCODE_SRC" "$TEST_SRC" "$VSCODE_DST" "$TEST_DST"; do
  if [[ ! -e "$dir" ]]; then
    echo "Error: 경로를 찾을 수 없습니다: $dir" >&2
    exit 1
  fi
done

# macOS vs Linux 인라인 편집 옵션 분기
if [[ "$(uname)" == "Darwin" ]]; then
  SED_INLINE=(-i "")
else
  SED_INLINE=(-i)
fi


# 1) VS Code 설정 복사 + gdb path 패치
echo "→ Updating $VSCODE_DST/launch.json and tasks.json"
cp -f "$VSCODE_SRC/launch.json" "$VSCODE_DST/launch.json"

sed "${SED_INLINE[@]}" -E \
  "s#(\"miDebuggerPath\"[[:space:]]*:[[:space:]]*\")[^\"]*\"#\1${GDB_PATH}\"#" \
  "$VSCODE_DST/launch.json"

cp -f "$VSCODE_SRC/tasks.json"  "$VSCODE_DST/tasks.json"

# 2) test 파일 복사
echo "→ Updating $TEST_DST/Makefile and visualize-rbtree.c"
cp -f "$TEST_SRC/Makefile"           "$TEST_DST/Makefile"
cp -f "$TEST_SRC/visualize-rbtree.c" "$TEST_DST/visualize-rbtree.c"

echo "✔ setup-docker complete."
