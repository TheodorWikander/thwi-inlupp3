#!/bin/bash
mkdir -p profiles
: > profiles/summary.txt

for f in inputs/*; do
  name=$(basename "$f")
  unique=$(./freq_count_cg "$f" | wc -l)

  valgrind --tool=callgrind --callgrind-out-file="profiles/cg.$name.out" \
      ./freq_count_cg "$f" > /dev/null 2> /dev/null

  callgrind_annotate "profiles/cg.$name.out" > "profiles/$name.txt"

  echo "== $name: $unique unika ord ==" >> profiles/summary.txt
  grep 'PROGRAM TOTALS' "profiles/$name.txt" | head -n 1 >> profiles/summary.txt
  grep -E '^ *[0-9,]+ \(' "profiles/$name.txt" | grep -v 'PROGRAM TOTALS' | head -n 3 >> profiles/summary.txt
  echo >> profiles/summary.txt
done

cat profiles/summary.txt
