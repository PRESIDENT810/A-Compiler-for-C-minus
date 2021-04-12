#ifndef PROJECT3_GRAMMER_H_
#define PROJECT3_GRAMMER_H_

#include "symbol.h"
#include <unordered_set>
#include <vector>
#include <list>

bool isTerminal(Symbol symbol);

class Rule{
public:
    Symbol LHS;
    std::vector<Symbol> RHS;
    explicit Rule(Symbol symbol){
        this->LHS = symbol;
        this->RHS = std::vector<Symbol>();
    }
};

class First{
public:
    Symbol symbol;
    std::unordered_set<Symbol> firstSet;
    explicit First(Symbol symbol){
        this->symbol = symbol;
        if (!isTerminal(symbol)){ // non-terminal symbol
            this->firstSet = std::unordered_set<Symbol>();
        } else{ // terminal symbol
            this->firstSet = std::unordered_set<Symbol>();
            this->firstSet.emplace(symbol);
        }
    }
    void takeUnion(const std::unordered_set<Symbol>& newSet){ // union the value of firstSet with the current guess
        for (auto element : newSet){
            this->firstSet.emplace(element);
        }
    }
    bool containNull(){
        for (auto s : this->firstSet){
            if (s == Symbol::nullStr) return true;
        }
        return false;
    }
};

class Follow{
public:
    Symbol symbol;
    std::unordered_set<Symbol> followSet;
    explicit Follow(Symbol symbol){
        this->symbol = symbol;
        this->followSet = std::unordered_set<Symbol>();
    }
    void takeUnion(const std::unordered_set<Symbol>& newSet){
        for (auto element : newSet){
            this->followSet.emplace(element);
        }
    }
};

class LL1Table{
public:
    Rule*** table; // the transition table for LL(1) parser
    LL1Table(const std::vector<First*>& Firsts, const std::vector<Follow*>& Follows, const std::vector<Rule*>& rules);
    Rule* findProduction(Symbol nonterminal, Symbol lookahead){
        return this->table[static_cast<int>(nonterminal)][static_cast<int>(lookahead)-21];
    }
};

void makeFirsts(std::vector<First*>* Firsts, std::vector<Rule*>* rules);

void makeFollows(std::vector<Follow*>* Follows, std::vector<First*>* Firsts, std::vector<Rule*>* rules);

void makeRules(std::vector<Rule*>* rules);

bool containsNull(const std::unordered_set<Symbol>& set);

std::unordered_set<Symbol> findFirst(std::list<Symbol> symbolList, std::vector<First*>* Firsts);

#endif //PROJECT3_GRAMMER_H

// The grammar we use:
//
// program: varDecl stmts

// TODO: common left prefix!
// varDecls: varDecl varDecls
// varDecls: valDecl
// change to:
// valDecls: varDecl valDeclsPostfix
// valDeclsPostfix: varDecls
// valDeclsPostfix: nullStr

// varDecl: INT declList SEMI

// TODO: common left prefix!
// declList: decl COMMA declList
// declList: decl
// change to:
// declList: decl declListPostfix
// declListPostfix: COMMA declList
// declListPostfix: nullStr

// TODO: common left prefix!
// decl: ID ASSIGN INTNUM
// decl: ID LSQUARE INTNUM RSQUARE
// decl: ID
// change to:
// decl: ID declPostfix
// declPostfix: ASSIGN INTNUM
// declPostfix: LSQUARE INTNUM RSQUARE
// declPostfix: nullStr

// codeBlock: stmt
// codeBlock: LBRACE stmts RBRACE

// TODO: common left prefix!
// stmts: stmt stmts
// stmts: stmt
// change to:
// stmts: stmt stmtsPostfix
// stmtsPostfix: stmts
// stmtsPostfix: nullStr

// stmt: assignStmt SEMI
// stmt: controlStmt
// stmt: rwStmt SEMI
// stmt: SEMI
// controlStmt: ifStmts
// controlStmt: whileStmt
// controlStmt: dowhileStmt SEMI
// controlStmt: returnStmt SEMI
// rwStmt: rStmt
// rwStmt: wStmt

// TODO: common left prefix!
// assignStmt: ID LSQUARE exp RSQUARE ASSIGN exp
// assignStmt: ID ASSIGN exp
// change to:
// assignStmt: ID assignStmtPostfix
// assignStmtPostfix: LSQUARE exp RSQUARE ASSIGN exp
// assignStmtPostfix: ASSIGN exp

// TODO: common left prefix!
// ifStmts: ifStmt
// ifStmts: ifStmt ELSE codeBlock
// change to:
// ifStmts: ifStmt ifStmtsPostfix
// ifStmtsPostfix: nullStr
// ifStmtsPostfix: ELSE codeBlock

// ifStmt: IF LPAR exp RPAR codeBlock
// whileStmt: WHILE LPAR exp RPAR codeBlock
// dowhileStmt: DO codeBlock WHILE LPAR exp RPAR
// returnStmt: RETURN
// rStmt: READ LPAR ID RPAR
// wStmt: WRITE LPAR exp RPAR
// exp: realExp OpExp

// TODO: common left prefix!
// realExp: ID
// realExp: ID LSQUARE exp RSQUARE
// change to:
// realExp: ID realExpPostfix
// realExpPostfix: nullStr
// realExpPostfix: LSQUARE exp RSQUARE

// realExp: NOT_OP exp
// realExp: LPAR exp RPAR
// realExp: MINUS exp
// OpExp: nullStr
// OpExp: AND_OP realExp OpExp
// OpExp: OR_OP realExp OpExp
// OpExp: PLUS realExp OpExp
// OpExp: MINUS realExp OpExp
// OpExp: MUL_OP realExp OpExp
// OpExp: DIV_OP realExp OpExp
// OpExp: LT realExp OpExp
// OpExp: GT realExp OpExp
// OpExp: EQ realExp OpExp
// OpExp: NOTEQ realExp OpExp
// OpExp: LTEQ realExp OpExp
// OpExp: GTEQ realExp OpExp
// OpExp: SHL_OP realExp OpExp
// OpExp: SHR_OP realExp OpExp
// OpExp: ANDAND realExp OpExp
// OpExp: OROR realExp OpExp