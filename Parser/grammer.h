#ifndef PROJECT3_GRAMMER_H_
#define PROJECT3_GRAMMER_H_

#include "symbol.h"
#include <unordered_set>
#include <vector>
#include <list>

bool isTerminal(Symbol symbol);

//
// class representing a production rule
//
class Rule{
public:
    Symbol LHS;
    std::vector<Symbol> RHS;
    explicit Rule(Symbol symbol){
        this->LHS = symbol;
        this->RHS = std::vector<Symbol>();
    }
};

//
// class representing the FIRST class
//
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

//
// class representing the FOLLOW class
//
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

//
// the analysis table for LL(1) parser
//
class LL1Table{
public:
    Rule*** table; // the transition table for LL(1) parser
    LL1Table(std::vector<First*>* Firsts, std::vector<Follow*>* Follows, std::vector<Rule*>* rules);
    Rule* findProduction(Symbol nonterminal, Symbol lookahead){
        return this->table[static_cast<int>(nonterminal)][static_cast<int>(lookahead)-nonTerminalCnt];
    }
};

//
// generate FIRST for all symbols
//
void makeFirsts(std::vector<First*>* Firsts, std::vector<Rule*>* rules);

//
// generate FOLLOW for all symbols
//
void makeFollows(std::vector<Follow*>* Follows, std::vector<First*>* Firsts, std::vector<Rule*>* rules);

//
// generate all production rules
//
void makeRules(std::vector<Rule*>* rules);

//
// whether a set contains nullStr symbol
//
bool containsNull(const std::unordered_set<Symbol>& set);

//
// find the FIRST for a series of symbols
//
std::unordered_set<Symbol> findFirst(std::list<Symbol> symbolList, std::vector<First*>* Firsts);

#endif //PROJECT3_GRAMMER_H

