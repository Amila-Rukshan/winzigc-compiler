#!/bin/bash

generate_cfg() {
    local winzigc_prog_path=$1
    local opt_flag=$2

    if [ -z "$opt_flag" ]; then
        echo "opt_flag is empty"
    elif [ "$opt_flag" = "opt" ]; then
        ./bazel-bin/winzigc/main/cmd -opt "$winzigc_prog_path"
    else
        ./bazel-bin/winzigc/main/cmd "$winzigc_prog_path"
    fi

    program_artefact_path="$(pwd)/cfg/$winzig_prog_name"/$opt_flag
    mkdir -p "$program_artefact_path"
    cd "$program_artefact_path" || exit
    opt -p='dot-cfg' -disable-output "$winzigc_prog_path".ll
    find "$program_artefact_path" -name "*.dot" -exec bash -c 'dot -Tpng "$0" > "$(basename ${0%.dot}).cfg.png"' {} \;
    find "$program_artefact_path" -type f -name "*.dot" -delete
    cd "$curr_dir" || exit
}

for i in {1..25}; do
    winzig_prog_name=$(printf "winzig_%02d" "$i")
    curr_dir="$(pwd)"
    winzigc_prog_path="$(pwd)/example-programs/$winzig_prog_name"

    generate_cfg "$winzigc_prog_path" "opt"
    generate_cfg "$winzigc_prog_path" "no-opt"

    rm "$winzigc_prog_path".ll
done
