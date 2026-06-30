#!/bin/bash -eu

# Build the library object files
$CXX $CXXFLAGS -c -I. src/parser.cpp -o parser.o
$CXX $CXXFLAGS $LIB_FUZZING_ENGINE fuzz/image_fuzzer.cc parser.o -o $OUT/image_fuzzer

$CXX $CXXFLAGS -c -I. src/archive.cpp -o archive.o
$CXX $CXXFLAGS $LIB_FUZZING_ENGINE fuzz/archive_fuzzer.cc archive.o -o $OUT/archive_fuzzer