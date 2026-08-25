#!/usr/bin/env bash
# Merges rathena/rathena master into the current branch.
#
# The fork is clang-formatted and its docs are Markdown; upstream is neither,
# so a plain merge would conflict on every line upstream touched. This script
# formats upstream's tree with the fork's .clang-format and converts its
# doc/*.txt with tools/doc2md.py first, and merges *that*, so the merge only
# has to reconcile real changes.
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
	tools="tools/format.sh tools/doc2md.py"
	git checkout -q "$target" -- .clang-format $tools
	tools/format.sh
	tools/doc2md.py --all --delete
	# put upstream's own version of the tool files back (or drop them if upstream has none)
	git reset -q -- .clang-format $tools
	for f in .clang-format $tools; do
		git checkout -q -- "$f" 2> /dev/null || rm -f "$f"
	done
	git add -A src doc
	git commit -q -m "clang-format upstream $(git rev-parse --short "upstream/$ref")"
	git checkout -q "$target"
fi

# converted docs can be less than 50% similar to their .txt source; lower the rename threshold
git merge -X find-renames=30% "$tmp"
echo "merged $tmp into $target; delete it with: git branch -D $tmp"
