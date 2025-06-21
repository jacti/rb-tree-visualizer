#!/usr/bin/env bash
set -euo pipefail

# ── Usage ──
if [[ $# -ne 2 ]]; then
  cat << EOF
Usage: $0 <project_root_dir> <gdb_path>
예: $0 /path/to/project_root /usr/bin/gdb
EOF
  exit 1
fi

# ── 인자 수집 ──
PRJ_ROOT_DIR="$1"
GDB_PATH="$2"

# ── 경로 계산 ──
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PLUGIN_ROOT="$(dirname "$SCRIPT_DIR")"

ORIGIN_ROOT="$SCRIPT_DIR"
SRC_SRC="$SCRIPT_DIR/src"
VSCODE_SRC="$SCRIPT_DIR/vscode"
TEST_SRC="$PLUGIN_ROOT/visualizer/test"

DST_ROOT="$PRJ_ROOT_DIR"
DST_SRC="$PRJ_ROOT_DIR/src"
DST_VSCODE="$PRJ_ROOT_DIR/.vscode"
DST_TEST="$PRJ_ROOT_DIR/test"

# ── 경로 유효성 검사 ──
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

# macOS vs Linux 인라인 편집 옵션 분기
if [[ "$(uname)" == "Darwin" ]]; then
  SED_INLINE=(-i "")
else
  SED_INLINE=(-i)
fi


# ── 1) 루트 Makefile 복사 ──
echo "→ Copying Makefile to $DST_ROOT/Makefile"
cp -f "$ORIGIN_ROOT/Makefile" "$DST_ROOT/Makefile"

# ── 2) src/Makefile 복사 ──
echo "→ Copying src/Makefile to $DST_SRC/Makefile"
cp -f "$SRC_SRC/Makefile" "$DST_SRC/Makefile"

# 3) VS Code 설정 복사 + gdb path 패치
echo "→ Updating VS Code settings in $DST_VSCODE"
cp -f "$VSCODE_SRC/launch.json" "$DST_VSCODE/launch.json"

# 패치: "miDebuggerPath": "<OLD>" → "miDebuggerPath": "<GDB_PATH>"
sed "${SED_INLINE[@]}" -E \
  "s#(\"miDebuggerPath\"[[:space:]]*:[[:space:]]*\")[^\"]*\"#\1${GDB_PATH}\"#" \
  "$DST_VSCODE/launch.json"

cp -f "$VSCODE_SRC/tasks.json"  "$DST_VSCODE/tasks.json"

# ── 4) test 파일 복사 ──
echo "→ Copying visualize files to $DST_TEST"
cp -f "$TEST_SRC/Makefile"           "$DST_TEST/Makefile"
cp -f "$TEST_SRC/visualize-rbtree.c" "$DST_TEST/visualize-rbtree.c"

echo "✔ setup-origin complete."
