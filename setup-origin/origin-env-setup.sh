#!/usr/bin/env bash
set -euo pipefail

# ── Usage ──
if [[ $# -lt 2 || $# -gt 3 ]]; then
  cat << EOF
Usage: $0 <project_root_dir> [rbtree_lab_folder] <gdb_path>
예1: $0 /path/to/project_root /usr/bin/gdb
예2: $0 /path/to/project_root rbtree_lab /usr/bin/gdb
EOF
  exit 1
fi

# ── 인자 분리 ──
PRJ_ROOT_DIR="$1"
if [[ $# -eq 3 ]]; then
  RBTREE_DIR="$2"
  GDB_PATH="$3"
else
  RBTREE_DIR=""
  GDB_PATH="$2"
fi

# ── 플러그인 경로 계산 ──
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PLUGIN_ROOT="$(dirname "$SCRIPT_DIR")"

# ── 소스 디렉터리 ──
ORIGIN_ROOT="$SCRIPT_DIR"
SRC_SRC="$SCRIPT_DIR/src"
VSCODE_SRC="$SCRIPT_DIR/vscode"
TEST_SRC="$PLUGIN_ROOT/visualizer/test"

# ── 대상 디렉터리 (DST_* 형식) ──

# src와 test 모두 RBTREE_DIR 유무에 따라 분기
if [[ -n "$RBTREE_DIR" ]]; then
  RB_ROOT_DST="$PRJ_ROOT_DIR/$RBTREE_DIR"
else
  RB_ROOT_DST="$PRJ_ROOT_DIR"
fi
SRC_DST="$RB_ROOT_DST/src"
TEST_DST="$RB_ROOT_DST/test"
VSCODE_DST="$PRJ_ROOT_DIR/.vscode"

# ── 경로 유효성 검사 ──
for path in \
  "$ORIGIN_ROOT/Makefile" \
  "$SRC_SRC/Makefile" \
  "$VSCODE_SRC/launch.json" \
  "$VSCODE_SRC/tasks.json" \
  "$TEST_SRC" \
  "$RB_ROOT_DST" \
  "$SRC_DST" \
  "$VSCODE_DST" \
  "$TEST_DST"; do
  if [[ ! -e "$path" ]]; then
    echo "Error: 경로를 찾을 수 없습니다: $path" >&2
    exit 1
  fi
done

# ── sed 인라인 옵션 분기 (macOS vs Linux) ──
if [[ "$(uname)" == "Darwin" ]]; then
  SED_INLINE=(-i "")
else
  SED_INLINE=(-i)
fi

# ── 1) 루트 Makefile 복사 ──
echo "→ Copying Makefile to $RB_ROOT_DST/Makefile"
cp -f "$ORIGIN_ROOT/Makefile" "$RB_ROOT_DST/Makefile"

# ── 2) src/Makefile 복사 ──
echo "→ Copying src/Makefile to $SRC_DST/Makefile"
cp -f "$SRC_SRC/Makefile" "$SRC_DST/Makefile"

# ── 3) VS Code 설정 복사 + gdb path 패치 ──
echo "→ Updating VS Code settings in $VSCODE_DST"
cp -f "$VSCODE_SRC/launch.json" "$VSCODE_DST/launch.json"
sed "${SED_INLINE[@]}" -E \
  "s#(\"miDebuggerPath\"[[:space:]]*:[[:space:]]*\")[^\"]*\"#\1${GDB_PATH}\"#" \
  "$VSCODE_DST/launch.json"
cp -f "$VSCODE_SRC/tasks.json"  "$VSCODE_DST/tasks.json"

# ── 3.1) workspaceFolder 치환 (RBTREE_DIR 있을 때만) ──
if [[ -n "${RBTREE_DIR:-}" ]]; then
  echo "→ Patching \${workspaceFolder} → \${workspaceFolder}/$RBTREE_DIR"
  for f in launch.json tasks.json; do
    sed "${SED_INLINE[@]}" \
      's|\${workspaceFolder}|${workspaceFolder}/'"$RBTREE_DIR"'|g' \
      "$VSCODE_DST/$f"
  done
fi

# ── 4) test 파일 패치 ──
"$PLUGIN_ROOT/visualizer/copy-test.sh" "$TEST_DST"

echo "✔ setup-origin complete."