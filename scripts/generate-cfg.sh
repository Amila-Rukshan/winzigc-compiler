#!/bin/sh

clang -emit-llvm -S -o test.ll test.c

opt -p='dot-cfg' -disable-output test.ll 

dot -Tpng .foo.dot > foo.cfg.png
