#include "VM.h"

VM::VM(){
    this->memTable = std::unordered_map<char*, Ptr*>();
    this->stackTop = 0;
}

int VM::allocate(int space){
    int res = this->stackTop;
    this->stackTop += space;
    return res;
}

Ptr* VM::registerVar(char* varName){
    int space = 4;
    Ptr* ptr = new VarPtr(this->allocate(space));
    this->memTable[varName] = ptr;
    return Ptr;
}

Ptr* VM::registerArr(char* arrName, int len){
    int space = 4*len;
    Ptr* ptr = new ArrPtr(this->allocate(space), len);
    this->memTable[arrName] = ptr;
    return ptr;
}

Ptr* VM::getPtr(char* name){
    return this->memTable[name];
}