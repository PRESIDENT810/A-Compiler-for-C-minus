#!/usr/bin/env bash
rm ./mips.s

clang++ Scanner/Scan.cpp Scanner/DFA.cpp Scanner/NFA.cpp Scanner/regex.cpp Parser/grammer.cpp Parser/Parse.cpp Parser/fixTree.cpp CodeGen/CogeGen.cpp CodeGen/VM.cpp main.cpp -std=c++17
./a.out >> ./mips.s

python3 ./Simulator/simulator.py