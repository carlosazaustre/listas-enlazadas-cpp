#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
build_dir=$(mktemp -d "${TMPDIR:-/tmp}/linked-list-demo.XXXXXX")
compiler=${CXX:-clang++}
flags=(-std=c++17 -Wall -Wextra -Wpedantic -Werror -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer)
for source in 01-nodes.cpp 02-link-order.cpp 03-ticket-list.cpp 04-standard-containers.cpp 05-count-search.cpp tests.cpp; do
  binary="$build_dir/${source%.cpp}"
  "$compiler" "${flags[@]}" "$source" -o "$binary"
  printf '\n%s\n' "$source"
  "$binary"
done
printf '\nBinarios temporales: %s\n' "$build_dir"
