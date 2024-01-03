#!/bin/bash

for i in $(seq -f "%02g" 0 0)
do
  echo "Running test $i"
  export GLOG_logtostderr=1
  ./bazel-bin/main/cmd /Users/amilarukshansenadheera/Desktop/Projcts/winzigc-compiler/tests/winzig_"$i"
done
