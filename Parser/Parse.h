#ifndef PROJECT3_PARSE_H_
#define PROJECT3_PARSE_H_

#include "grammer.h"

Symbol inferType(char* tokenValue);

class Token{
public:
    Symbol type;
    char* semanticValue;
    Token(char* semanticValue, char* tokenValue){
        this->semanticValue = new char[20];
        strcpy(this->semanticValue, semanticValue);
        this->type = inferType(tokenValue);
    }
};

class TreeNode;

class TreeNode{
public:
    Symbol mySymbol;
    TreeNode* myParent;
    std::list<TreeNode*> myChildren;
    explicit TreeNode(Symbol mySymbol, TreeNode* myParent){
        this->mySymbol = mySymbol;
        this->myParent = myParent;
    }
};

#endif //PROJECT3_PARSE_H