#! /bin/bash

ARCH=""
OS=""
CMD="g++ src/main.cpp -o build/out -lraylib -Wextra -Wall"

for arg in "$@"; do
    if [[ "$arg" == "-A"* ]]; then
        ARCH="${arg:2}"
        continue
    fi
    if [[ "$arg" == "-O"* ]]; then
        OS="${arg:2}"
        continue
    fi
    CMD+=" -D$arg"
done

echo "$CMD"
$CMD
if [ $? -eq 0 ]; then
    if [[ $ARCH == "" ]] || [[ $OS == "" ]]; then
        echo -e "\e[31m!!! Architecture or OS not specified; not creating release archive\e[0m"
    else
        echo -e "\e[32m    Creating release for $ARCH on $OS\e[0m (that should match this architecture)"
        rm build/*.tar.gz
        tar -czvf build/$ARCH-$OS.tar.gz build/*
    fi
    echo -e "\e[32m    Build succeeded\e[0m"
    ./build/out
else
    echo -e "\e[31m!!! Build FAILED !!!\e[0m"
fi
