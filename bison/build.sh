#!/bin/sh

mkdir -p obj bin

bison -o obj/rpcalc.c src/rpcalc.y
gcc -Wall -o bin/rpcalc obj/rpcalc.c -lm

CXXFLAGS="-Wall -I ./src/ -Wno-unused -Wno-deprecated  -Wno-write-strings -Wno-free-nonheap-object"

bison -d -v -y -b cool --debug -p cool_yy -o obj/cool-bison-parser.cc src/cool.bison
flex -d -o obj/cool-flex-lexer.cc ./src/cool.flex

g++ $LDFLAGS $CXXFLAGS src/parser-phase.cc src/utilities.cc src/stringtab.cc src/cool-tree.cc obj/cool-flex-lexer.cc obj/cool-bison-parser.cc -o bin/parser
