#!/usr/bin/bash

c_exe=".c"
test_dir="./test/"
build_dir="./bin/"

testfile="$test_dir$1$c_exe"
buildfile="$build_dir$1"

gcc $testfile -o $buildfile
$buildfile
