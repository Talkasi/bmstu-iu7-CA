#!/bin/bash

mkdir -p ./out/
rm ./out/*

g++ -g3 *.cpp -o ./out/app.exe

./out/app.exe
