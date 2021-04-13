#ifndef PROJECT3_PARSE_H_
#define PROJECT3_PARSE_H_

#include "grammer.h"

Symbol inferType(char* tokenValue);

//
// Token class stores the information of a token
//
class Token{
public:
    Symbol type;
    char* semanticValue;
    Token(char* semanticValue, char* tokenValue){
        this->semanticValue = new char[20];
        strcpy(this->semanticValue, semanticValue);
        this->type = inferType(tokenValue);
    }
    Token(){
        this->semanticValue = new char[0];
        this->type = Symbol::nullStr;
    }
};

class TreeNode;

//
// a node in the parse tree
//
class TreeNode{
public:
    int depth;
    Symbol mySymbol;
    TreeNode* myParent;
    std::list<TreeNode*> myChildren;
    explicit TreeNode(Symbol mySymbol, TreeNode* myParent, int depth){
        this->mySymbol = mySymbol;
        this->myParent = myParent;
        this->depth = depth;
    }
};

#endif //PROJECT3_PARSE_H