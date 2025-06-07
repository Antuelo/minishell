#!/bin/bash

TEST_FILE=tests/test_cases.txt
OUT_DIR=tests/expected
i=1

mkdir -p $OUT_DIR

while IFS= read -r line
do
    printf "%s\n" "$line" | bash > "$OUT_DIR/test_$i.txt" 2>&1
    echo "Saved expected: test_$i.txt ← $line"
    ((i++))
done < "$TEST_FILE"
