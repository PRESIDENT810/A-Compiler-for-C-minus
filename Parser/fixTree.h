#ifndef PROJECT3_FIXTREE_H
#define PROJECT3_FIXTREE_H

#include "Parse.h"
#include <unordered_map>

class PrecMap{
public:
    std::unordered_map<Symbol, int>map;
    PrecMap(){
        map[Symbol::DIV_OP] = 3;
        map[Symbol::MUL_OP] = 3;
        map[Symbol::PLUS] = 4;
        map[Symbol::MINUS] = 4;
        map[Symbol::SHL_OP] = 5;
        map[Symbol::SHR_OP] = 5;
        map[Symbol::GT] = 6;
        map[Symbol::GTEQ] = 6;
        map[Symbol::LT] = 6;
        map[Symbol::LTEQ] = 6;
        map[Symbol::EQ] = 7;
        map[Symbol::NOTEQ] = 7;
        map[Symbol::AND_OP] = 8;
        map[Symbol::OR_OP] = 10;
        map[Symbol::ANDAND] = 11;
        map[Symbol::OROR] = 12;
    }
    int lookup(Symbol symbol){
        return map[symbol];
    }
};

void removePostfix(TreeNode* crtNode);

bool isOperator(TreeNode* node);

void swapOperator(TreeNode* crtNode);

void concatExp(TreeNode* crtNode);

void fixUnary(TreeNode* crtNode);

inline bool ifHigherPrecedence(TreeNode* node1, TreeNode* node2);

void fixPrecedence(TreeNode* crtNode);

void renameExp(TreeNode* crtNode);

void resetParent(TreeNode* crtNode);

void fixAST(TreeNode* root);

#endif //PROJECT3_FIXTREE_H
