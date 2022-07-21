#!/usr/bin/bash

c_exe=".c"
test_dir="./test/"
build_dir="./bin/"
common="./src/common/error.c"

testfile="$test_dir$1$c_exe"
buildfile="$build_dir$1"

gcc -I./src/ $common $testfile -o $buildfile
$buildfile $2 $3
