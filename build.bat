@echo off
echo BUILDING...
g++ -Wall .\source\main.cpp -o aoc.exe -l ws2_32 -g
echo DONE!