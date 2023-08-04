@echo off
if not exist flecs.o gcc -c ../src/flecs.c -I../include -std=gnu99
g++ ../src/main.cpp flecs.o -o ../build/main -I../include -std=c++0x -lWs2_32