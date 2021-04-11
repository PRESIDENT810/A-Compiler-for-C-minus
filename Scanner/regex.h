#ifndef UNTITLED_REGEX_H
#define UNTITLED_REGEX_H

#include "NFA.h"

//
// this function generates a NFA for a reserved word
//
NFA* reservedGen(const char* reservedWord, const char* token);

//
// this function generates a NFA for a symbol
//
NFA* symbolGen(const char* symbol, const char* token);

//
// this function generates a NFA for a digit: 0-9
//
NFA* digitGen();

//
// this function generates a NFA for a letter: A-Z, a-z
//
NFA* letterGen();

//
// this function generates a NFA for a INTNUM: (+|-)?.digit.digit*
//
NFA* intnumGen();

//
// this function generate a NFA for a ID: (letter+).(digit|letter|_)*
//
NFA* idGen();

//
// this function generate NFAs for all reserved words
//
std::vector<NFA*> allReservedWordGen();

//
// this function generate NFAs for all reserved words
//
std::vector<NFA*> allSymbolGen();



#endif //UNTITLED_REGEX_H
