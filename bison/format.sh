#!/bin/bash

FILES=`find -maxdepth 4 -type f -name "*.cpp" -o -name "*.hpp" -o -name "*.c" -o -name "*.h"`

clang-format -i ${FILES}

if [ "$1" = "-e" ]
then 
echo -e "Formatted:\n${FILES}"
fi
