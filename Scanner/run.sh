#!/usr/bin/env bash
g++ Scan.cpp regex.cpp NFA.cpp DFA.cpp -std=c++14
./a.out >> ../ScannerOutput.txt