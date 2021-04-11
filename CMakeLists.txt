cmake_minimum_required(VERSION 3.13)
project(Project3)

set(CMAKE_CXX_STANDARD 17)

add_executable(Project3
#        Scanner/Scan.cpp
#        Scanner/DFA.cpp
#        Scanner/NFA.cpp
#        Scanner/regex.cpp
        Parser/grammer.cpp
        Parser/grammer.h
        Parser/Parse.cpp Parser/Parse.h)