#!/bin/bash

SCRIPT_DIR=$( cd -- "$(dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)

for entry in "$SCRIPT_DIR"/apps/*
do
    IFS='/'
    read -ra arr <<< "$entry"
    i686-rbos-g++ "$entry"/main.cpp -o "${SCRIPT_DIR}/../../base/bin/"${arr[${#arr[@]} - 1]}".o" start.o
done
