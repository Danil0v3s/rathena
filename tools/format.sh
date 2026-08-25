#!/usr/bin/env bash
# Formats (or with --check, verifies) every C++ source file with clang-format
# using the rules in .clang-format. Requires clang-format 22.
#
#   tools/format.sh            format in place
#   tools/format.sh --check    exit 1 and list files that are not formatted
#   tools/format.sh path...    limit to the given files/directories

set -euo pipefail
cd "$(dirname "$0")/.."

CLANG_FORMAT="${CLANG_FORMAT:-clang-format}"
mode=format
paths=()
for arg in "$@"; do
	case "$arg" in
		--check) mode=check ;;
		*) paths+=("$arg") ;;
	esac
done
if [ ${#paths[@]} -eq 0 ]; then
	paths=(src)
fi

if ! "$CLANG_FORMAT" --version | grep -q "version 22\."; then
	echo "warning: $("$CLANG_FORMAT" --version) - the project is formatted with clang-format 22, output may differ" >&2
fi

mapfile -t files < <(git ls-files -- "${paths[@]}" | grep -E '\.(cpp|hpp|inc)$')

if [ "$mode" = check ]; then
	# .inc files have no extension clang-format knows, treat them as C++
	status=0
	for f in "${files[@]}"; do
		if ! "$CLANG_FORMAT" --dry-run -Werror --style=file --assume-filename="${f%.inc}.cpp" < "$f" > /dev/null 2>&1; then
			echo "not formatted: $f"
			status=1
		fi
	done
	exit $status
fi

# clang-format occasionally needs a second pass to reach a fixed point
# (trailing comments, comment-only lines in case blocks); run until stable.
for pass in 1 2 3; do
	before=$(git diff --no-ext-diff -- "${files[@]}" | md5sum)
	printf '%s\n' "${files[@]}" | grep -v '\.inc$' | xargs -P "$(nproc)" -n 50 "$CLANG_FORMAT" -i --style=file
	for f in "${files[@]}"; do
		case "$f" in
			*.inc) "$CLANG_FORMAT" --style=file --assume-filename="${f%.inc}.cpp" < "$f" > "$f.tmp" && mv "$f.tmp" "$f" ;;
		esac
	done
	after=$(git diff --no-ext-diff -- "${files[@]}" | md5sum)
	[ "$before" = "$after" ] && break
done
echo "formatted ${#files[@]} files"
