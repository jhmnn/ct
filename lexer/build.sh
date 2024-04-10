#!/bin/sh

mkdir -p bin

flex++ -o ./src/CoolLexer.cpp ./src/CoolLexer.flex

g++ -Wall ./src/driver.cpp ./src/CoolLexer.cpp -o bin/driver
