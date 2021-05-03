#ifndef PROJECT3_VM_H
#define PROJECT3_VM_H

#include <unordered_map>

enum class Type{
    integer = 0,
    array = 1
};

class Ptr{
public:
    Type type;
    virtual int getAddr() = 0;
};

class VarPtr: public Ptr{
public:
    int baseAddr;
    explicit VarPtr(int addr){
        this->baseAddr = addr;
        this->type = Type::integer;
    }
    int getAddr() override{
        return this->baseAddr;
    }
};

class ArrPtr: public Ptr{
public:
    int baseAddr;
    int len;
    explicit ArrPtr(int addr, int len){
        this->baseAddr = addr;
        this->len = len;
        this->type = Type::array;
    }
    int getAddr() override{
        return this->baseAddr;
    }
};

class VM{
public:
    std::unordered_map<char*, Ptr*> memTable;
    int stackTop;
    explicit VM();
    Ptr* registerVar(char* varName);
    Ptr* registerArr(char* arrName, int len);
    Ptr* getPtr(char* name);
    int allocate(int space);
};

#endif //PROJECT3_VM_H
