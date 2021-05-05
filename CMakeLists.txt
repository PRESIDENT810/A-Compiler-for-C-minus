cmake_minimum_required(VERSION 3.13)
project(Project3)

set(CMAKE_CXX_STANDARD 17)

add_executable(Project4
        Scanner/Scan.cpp
        Scanner/DFA.cpp
        Scanner/NFA.cpp
        Scanner/regex.cpp
        Scanner/Scan.h

        Parser/grammer.cpp
        Parser/grammer.h
        Parser/Parse.cpp
        Parser/Parse.h
        Parser/fixTree.cpp
        Parser/fixTree.h

        CodeGen/CogeGen.cpp
        CodeGen/CogeGen.h
        CodeGen/VM.cpp
        CodeGen/VM.h

        main.cpp)