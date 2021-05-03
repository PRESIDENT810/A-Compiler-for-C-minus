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
    Symbol mySymbol;
    TreeNode* myParent;
    char* semanticValue;
    std::vector<TreeNode*> myChildren;
    explicit TreeNode(Symbol mySymbol, TreeNode* myParent){
        this->mySymbol = mySymbol;
        this->myParent = myParent;
        this->semanticValue = new char[10];
        strcpy(this->semanticValue, "\0");
    }
};

std::vector<Token*> tokenScanner(char* filename);

TreeNode* recursiveParse(Symbol crtSymbol, std::vector<Token*> tokenVec, TreeNode* parentNode);

void showTree(TreeNode* crtNode, int depth);

#endif //PROJECT3_PARSE_H