#!/usr/bin/env bash
# Merges rathena/rathena master into the current branch.
#
# The fork is clang-formatted and upstream is not, so a plain merge would
# conflict on every line upstream touched. This script formats upstream's
# tree with the fork's .clang-format first and merges *that*, so the merge
# only has to reconcile real code changes.
#
#   tools/merge-upstream.sh              merge upstream/master
#   tools/merge-upstream.sh <ref>        merge another upstream ref

set -euo pipefail
cd "$(dirname "$0")/.."

ref="${1:-master}"
git remote get-url upstream > /dev/null 2>&1 || git remote add upstream https://github.com/rathena/rathena.git
git fetch upstream "$ref"

target=$(git rev-parse --abbrev-ref HEAD)
tmp="upstream-formatted/$(git rev-parse --short "upstream/$ref")"

if git rev-parse --verify --quiet "$tmp" > /dev/null; then
	echo "using existing $tmp"
else
	git checkout -q -b "$tmp" "upstream/$ref"
	# format with the fork's rules, not whatever upstream may have
	git checkout -q "$target" -- .clang-format tools/format.sh
	tools/format.sh
	# put upstream's own version of the two files back (or drop them if upstream has none)
	git reset -q -- .clang-format tools/format.sh
	for f in .clang-format tools/format.sh; do
		git checkout -q -- "$f" 2> /dev/null || rm -f "$f"
	done
	git add -A src
	git commit -q -m "clang-format upstream $(git rev-parse --short "upstream/$ref")"
	git checkout -q "$target"
fi

git merge "$tmp"
echo "merged $tmp into $target; delete it with: git branch -D $tmp"
