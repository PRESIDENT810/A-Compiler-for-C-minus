#!/usr/bin/env bash
g++ Scan.cpp regex.cpp NFA.cpp DFA.cpp -std=c++14
rm ../ScannerOutput.txt
./a.out >> ../ScannerOutput.txt