#!/bin/bash

check_exit_status() {
    if [ $? -ne 0 ]; then
        echo "$1"
        exit 1
    fi
}

winzigc_prog_name=$1
dbg_flag=false
opt_flag=false

# Check for -dbg and -opt flags
for arg in "$@"; do
    if [ "$arg" = "-dbg" ]; then
        dbg_flag=true
    elif [ "$arg" = "-opt" ]; then
        opt_flag=true
    fi
done

if [ -z "$winzigc_prog_name" ]; then
    echo "Please provide a program name in the ./example-programs directory"
    exit 1
fi

winzigc_prog_path="$(pwd)/example-programs/$winzigc_prog_name"

export GLOG_logtostderr=1

cmd="./bazel-bin/winzigc/main/cmd"
if [ "$dbg_flag" = true ]; then
    cmd+=" -dbg"
fi
if [ "$opt_flag" = true ]; then
    cmd+=" -opt"
fi
cmd+=" \"$winzigc_prog_path\""
eval "$cmd"

check_exit_status "Failed to generate llvm bitcode!"

if [ "$dbg_flag" = true ]; then
    clang -x ir "$winzigc_prog_path".ll -o "$winzigc_prog_path"_binary -g
else
    clang -O3 "$winzigc_prog_path".ll -o "$winzigc_prog_path"_binary
fi

check_exit_status "Failed to generate binary using generatd llvm bitcode!"

echo "Program compiled successfully!"
echo "Executing the" "$winzigc_prog_name"_binary:

"$winzigc_prog_path"_binary

if [ "$dbg_flag" = false ]; then
    rm "$winzigc_prog_path".ll "$winzigc_prog_path"_binary
fi
