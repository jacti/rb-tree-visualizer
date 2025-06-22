#!/usr/bin/env bash
set -euo pipefail

# Usage
if [[ $# -ne 1 ]]; then
  cat << EOF >&2
Usage: $0 <test_dst_dir>
예: $0 /path/to/project/test
EOF
  exit 1
fi

TEST_DST="$1"

# 이 스크립트 위치 & test 소스 폴더
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEST_SRC="$SCRIPT_DIR/test"

# 1) 삭제 목록 파일 읽어서, 목적지에 남아있다면 제거
LEGACY_LIST="$SCRIPT_DIR/legacy-files.txt"
if [[ -f "$LEGACY_LIST" ]]; then
  echo "→ Removing legacy files in $TEST_DST"
  while IFS= read -r fname; do
    # 주석·빈줄 무시
    [[ "$fname" =~ ^# ]] && continue
    [[ -z "$fname" ]]   && continue

    dest="$TEST_DST/$fname"
    if [[ -e "$dest" ]]; then
      rm -f "$dest"
      echo "   removed $fname"
    fi
  done < "$LEGACY_LIST"
fi

# 2) 소스의 모든 파일을 목적지에 복사
echo "→ Copying current test files from $TEST_SRC to $TEST_DST"
for file in "$TEST_SRC"/*; do
  cp -f "$file" "$TEST_DST/$(basename "$file")"
done
