#include "fixTree.h"

extern std::vector<Rule*> rules;
extern std::vector<First*> Firsts;
extern std::vector<Follow*> Follows;
extern LL1Table* table;

void removePostfix(TreeNode* crtNode){
    for (auto node : crtNode->myChildren) removePostfix(node);
    // declList: decl declListPostfix -> declList: decl COMMA declList | decl
    // decl: ID declPostfix -> decl: ID ASSIGN INTNUM | LSQUARE INTNUM RSQUARE | ID
    // stmts: stmt stmtsPostfix -> stmts: stmt stmts | stmt
    // assignStmt: ID assignStmtPostfix -> assignStmt: ID LSQUARE exp RSQUARE ASSIGN exp | ID ASSIGN exp
    // ifStmts: ifStmt ifStmtsPostfix -> ifStmts: ifStmt | ifStmt ELSE codeBlock
    if (crtNode->mySymbol == Symbol::declList
        || crtNode->mySymbol == Symbol::decl
        || crtNode->mySymbol == Symbol::stmts
        || crtNode->mySymbol == Symbol::assignStmt
        || crtNode->mySymbol == Symbol::ifStmts){
        auto replaceChildren = std::vector<TreeNode*>();
        replaceChildren.emplace_back(crtNode->myChildren[0]);
        auto postfix = crtNode->myChildren[1];
        for (auto node : postfix->myChildren) {
            if (node->mySymbol != Symbol::nullStr) replaceChildren.emplace_back(node);
        }
        crtNode->myChildren.swap(replaceChildren);
    }
    // realExp: ID realExpPostfix -> realExp: ID | ID LSQUARE exp RSQUARE
    if (crtNode->mySymbol == Symbol::realExp && crtNode->myChildren.size() > 1 && crtNode->myChildren[1]->mySymbol == Symbol::realExpPostfix){
        auto replaceChildren = std::vector<TreeNode*>();
        replaceChildren.emplace_back(crtNode->myChildren[0]);
        auto postfix = crtNode->myChildren[1];
        for (auto node : postfix->myChildren) {
            if (node->mySymbol != Symbol::nullStr) replaceChildren.emplace_back(node);
        }
        crtNode->myChildren.swap(replaceChildren);
    }
}

bool isOperator(TreeNode* node){
    auto symbol = node->mySymbol;
    return symbol == Symbol::AND_OP
           || symbol == Symbol::OR_OP
           || symbol == Symbol::PLUS
           || symbol == Symbol::MINUS
           || symbol == Symbol::MUL_OP
           || symbol == Symbol::DIV_OP
           || symbol == Symbol::LT
           || symbol == Symbol::GT
           || symbol == Symbol::EQ
           || symbol == Symbol::NOTEQ
           || symbol == Symbol::LTEQ
           || symbol == Symbol::GTEQ
           || symbol == Symbol::SHL_OP
           || symbol == Symbol::SHR_OP
           || symbol == Symbol::ANDAND
           || symbol == Symbol::OROR;
}

void swapOperator(TreeNode* crtNode){
    auto replaceChildren = std::vector<TreeNode*>();
    for (auto child : crtNode->myChildren){
        if (child->mySymbol != Symbol::OpExp) replaceChildren.emplace_back(child);
        // exp: realExp OpExp -> exp: realExp;
        // OpExp: nullStr ->
        else if (child->myChildren[0]->mySymbol == Symbol::nullStr) break;
        else{
            // exp: realExp OpExp -> exp: realExp Op OpExp
            // OpExp: AND_OP realExp OpExp -> OpExp: realExp OpExp
            replaceChildren.emplace_back(child->myChildren[0]);
            auto replaceGrandson = std::vector<TreeNode*>();
            for (auto grandson : child->myChildren){
                if (grandson != child->myChildren[0]) replaceGrandson.emplace_back(grandson);
            }
            child->myChildren.swap(replaceGrandson);
            replaceChildren.emplace_back(child);
        }
    }
    crtNode->myChildren.swap(replaceChildren);
    for (auto child : crtNode->myChildren) swapOperator(child);
}

void concatExp(TreeNode* crtNode){
    if (crtNode->mySymbol == Symbol::exp || crtNode->mySymbol == Symbol::OpExp){
        if (crtNode->myChildren.size() == 1 && crtNode->myChildren[0]->mySymbol == Symbol::realExp){
            crtNode->myChildren = crtNode->myChildren[0]->myChildren;
            for (auto child : crtNode->myChildren) child->myParent = crtNode;
        }
    }
    for (auto child : crtNode->myChildren) concatExp(child);
}

void fixUnary(TreeNode* crtNode){
    for (auto child : crtNode->myChildren) fixUnary(child);
    if (crtNode->myChildren.size() == 2
        && (crtNode->myChildren[0]->mySymbol == Symbol::MINUS || crtNode->myChildren[0]->mySymbol == Symbol::NOT_OP)
        && crtNode->myChildren[1]->mySymbol == Symbol::exp
        && crtNode->myChildren[1]->myChildren.size() == 3){
        TreeNode* newCrtNode = crtNode->myChildren[1];
        crtNode->myChildren.swap(newCrtNode->myChildren);
        newCrtNode->myChildren[1] = crtNode->myChildren[0];
        crtNode->myChildren[0] = newCrtNode;
    }
}

inline bool ifHigherPrecedence(TreeNode* node1, TreeNode* node2){
    static PrecMap pm = PrecMap();
    return pm.lookup(node1->mySymbol) <= pm.lookup(node2->mySymbol);
}

void fixPrecedence(TreeNode* crtNode){
    for (auto child : crtNode->myChildren) fixPrecedence(child);
    if (crtNode->mySymbol == Symbol::exp && crtNode->myChildren.size() == 3 && crtNode->myChildren[2]->myChildren.size() == 3){
        if (ifHigherPrecedence(crtNode->myChildren[1], crtNode->myChildren[2]->myChildren[1])){
            TreeNode* newCrtNode = crtNode->myChildren[2];
            crtNode->myChildren.swap(newCrtNode->myChildren);
            newCrtNode->myChildren[2] = crtNode->myChildren[0];
            crtNode->myChildren[0] = newCrtNode;
        }
    }
}

void renameExp(TreeNode* crtNode){
    if (crtNode->mySymbol == Symbol::realExp || crtNode->mySymbol == Symbol::OpExp) crtNode->mySymbol = Symbol::exp;
    for (auto child : crtNode->myChildren) renameExp(child);
}

void resetParent(TreeNode* crtNode){
    for (auto child : crtNode->myChildren){
        child->myParent = crtNode;
        resetParent(child);
    }
}

void fixAST(TreeNode* root){
    removePostfix(root);
    swapOperator(root);
    concatExp(root);
    renameExp(root);
    fixUnary(root);
    fixPrecedence(root);
}

int main(){
    makeRules(&rules);
    makeFirsts(&Firsts, &rules);
    makeFollows(&Follows, &Firsts, &rules);
    table = new LL1Table(&Firsts, &Follows, &rules);
    char filename[30] = "../ScannerOutput.txt";
    std::vector<Token*> tokenVec = tokenScanner(filename);
    try {
        TreeNode* root = recursiveParse(Symbol::program, tokenVec, nullptr);
        fixAST(root);
        showTree(root, 0);
    } catch (std::exception &e){
        printf("%s", e.what());
        printf("Wrong grammar, cannot parse!\n");
    }
    return 0;
}
