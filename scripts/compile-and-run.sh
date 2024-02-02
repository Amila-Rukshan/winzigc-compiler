#!/bin/bash

check_exit_status() {
    if [ $? -ne 0 ]; then
        echo "$1"
        exit 1
    fi
}

winzigc_prog_name=$1

if [ -z "$winzigc_prog_name" ]; then
    echo "Please provide a program name in the ./tests directory"
    exit 1
fi

winzigc_prog_path="$(pwd)/tests/$winzigc_prog_name"

./bazel-bin/winzigc/main/cmd "$winzigc_prog_path" \
    &>"$winzigc_prog_path".ll

check_exit_status "Failed to generate llvm bitcode!"

clang -O3 "$winzigc_prog_path".ll \
    -o "$winzigc_prog_path"_binary \
    # -v

check_exit_status "Failed to generate binary using generatd llvm bitcode!"

echo "Program compiled successfully!"
echo "Executing the" "$winzigc_prog_name"_binary:

"$winzigc_prog_path"_binary

rm "$winzigc_prog_path".ll "$winzigc_prog_path"_binary
