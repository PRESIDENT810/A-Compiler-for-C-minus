#ifndef PROJECT3_SCAN_H
#define PROJECT3_SCAN_H

#include "DFA.h"

class inputBuffer{
public:
    char buffer[20]{};
    int ptr = 0;
    inputBuffer(){
        memset(this->buffer, '\0', 20);
    }
    void fillBuffer(char c){
        this->buffer[this->ptr++] = c;
    }
    void clearBuffer(){
        memset(this->buffer, '\0', 20);
        this->ptr = 0;
    }
    void printBuffer(FILE* file){
        fprintf(file, "%s\n", this->buffer);
    }
};

void prepareDFA();

bool findMatch(DFA_Node* dfa_node);

bool isBlank(char c);

inline char readNext(FILE *fp);

void readFile(FILE* fp);

#endif //PROJECT3_SCAN_H