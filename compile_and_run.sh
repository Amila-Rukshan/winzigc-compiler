#!/bin/bash

file_path="/Users/amilarukshansenadheera/Desktop/Projcts/winzigc-compiler/tests/winzig_zz"

./bazel-bin/winzigc/main/cmd "$file_path" \
    &>"$file_path".ll

clang -O3 "$file_path".ll \
    -o "$file_path"_binary \
    # -v

echo "Program compiled successfully."
echo "Executing the program:"

"$file_path"_binary
