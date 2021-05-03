#!/usr/bin/env bash
rm ./ScannerOutput.txt

clang++ ./Scanner/Scan.cpp ./Scanner/regex.cpp ./Scanner/NFA.cpp ./Scanner/DFA.cpp -std=c++17 -o ./Scanner/Scanner
./Scanner/Scanner >> ./ScannerOutput.txt

#g++ ./Parser/grammer.cpp ./Parser/Parse.cpp -std=c++17 -o ./Parser/Parser
#./Parser/Parser