#include "VM.h"

VM::VM(){
    this->memTable = std::unordered_map<std::string_view, Ptr*>();
    this->stackTop = 0;
    this->regUsed = 3;
    this->label = 0;
}

int VM::allocMem(int space){
    int res = this->stackTop;
    this->stackTop += space;
    return res;
}

int VM::allocReg(){
    this->regUsed++;
    return this->regUsed;
}

void VM::freeReg(){
    this->regUsed--;
}

int VM::newLabel(){
    this->label++;
    return this->label;
}

Ptr* VM::registerVar(char* varName){
    int space = 4;
    Ptr* ptr = new VarPtr(this->allocMem(space));
    this->memTable[varName] = ptr;
    return ptr;
}

Ptr* VM::registerArr(char* arrName, int len){
    int space = 4*len;
    Ptr* ptr = new ArrPtr(this->allocMem(space), len);
    this->memTable[arrName] = ptr;
    return ptr;
}

Ptr* VM::getPtr(char* name){
    return this->memTable[name];
}