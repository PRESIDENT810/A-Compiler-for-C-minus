#ifndef PROJECT3_VM_H
#define PROJECT3_VM_H

#include <string_view>
#include <unordered_map>

//
// identifier for type of variables
//
enum class Type{
    integer = 0,
    array = 1
};

//
// pointer in VM
//
class Ptr{
public:
    Type type;
    explicit Ptr(){
        this->type = Type::integer;
    }
    virtual int getAddr() = 0;
};

//
// pointer of variables in VM
//
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

//
// pointer of arrays in VM
//
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

//
// virtual machine
//
class VM{
public:
    std::unordered_map<std::string_view, Ptr*> memTable;
    int stackTop;
    int regUsed;
    int label;

    explicit VM();
    Ptr* registerVar(char* varName);
    Ptr* registerArr(char* arrName, int len);
    Ptr* getPtr(char* name);
    int allocMem(int space);

    int allocReg();
    void freeReg();

    int newLabel();
};

#endif //PROJECT3_VM_H
