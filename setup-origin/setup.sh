#!/usr/bin/env bash
set -euo pipefail

echo "Usage: $0 <project_root_dir> <gdb_path>"
if [[ $# -ne 2 ]]; then
  echo "Error: 프로젝트 루트 디렉터리와 gdb 경로를 인자로 전달해야 합니다." >&2
  echo "예: $0 /path/to/project_root /usr/bin/gdb" >&2
  exit 1
fi

PRJ_ROOT_DIR="$1"
GDB_PATH="$2"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PLUGIN_ROOT="$(dirname "$SCRIPT_DIR")"

# 소스
ORIGIN_ROOT="$SCRIPT_DIR"
SRC_SRC="$SCRIPT_DIR/src"
VSCODE_SRC="$SCRIPT_DIR/vscode"
TEST_SRC="$PLUGIN_ROOT/visualizer/test"

# 대상
DST_ROOT="$PRJ_ROOT_DIR"
DST_SRC="$PRJ_ROOT_DIR/src"
DST_VSCODE="$PRJ_ROOT_DIR/.vscode"
DST_TEST="$PRJ_ROOT_DIR/test"

# 필수 경로 확인
for dir in \
  "$ORIGIN_ROOT/Makefile" \
  "$SRC_SRC/Makefile" \
  "$VSCODE_SRC/launch.json" \
  "$VSCODE_SRC/tasks.json" \
  "$TEST_SRC/Makefile" \
  "$TEST_SRC/visualize-rbtree.c" \
  "$DST_ROOT" \
  "$DST_SRC" \
  "$DST_VSCODE" \
  "$DST_TEST"; do
  if [[ ! -e "$dir" ]]; then
    echo "Error: 경로를 찾을 수 없습니다: $dir" >&2
    exit 1
  fi
done

# 1) 루트 Makefile 복사
echo "→ Copying Makefile to $DST_ROOT/Makefile"
cp -f "$ORIGIN_ROOT/Makefile" "$DST_ROOT/Makefile"

# 2) src/Makefile 복사
echo "→ Copying src/Makefile to $DST_SRC/Makefile"
cp -f "$SRC_SRC/Makefile" "$DST_SRC/Makefile"

# 3) VSCode 설정 복사 + gdb path 패치
echo "→ Updating VS Code settings in $DST_VSCODE"
cp -f "$VSCODE_SRC/launch.json" "$DST_VSCODE/launch.json"
sed -i 's#\("miDebuggerPath"\s*:\s*"\)[^"]*\("\)#\1'"$GDB_PATH"'\2#' "$DST_VSCODE/launch.json"
cp -f "$VSCODE_SRC/tasks.json"  "$DST_VSCODE/tasks.json"

# 4) test 파일 복사
echo "→ Copying visualize files to $DST_TEST"
cp -f "$TEST_SRC/Makefile"           "$DST_TEST/Makefile"
cp -f "$TEST_SRC/visualize-rbtree.c" "$DST_TEST/visualize-rbtree.c"

echo "✔ setup-origin complete."
