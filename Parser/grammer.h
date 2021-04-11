#include "symbol.h"
#include <unordered_set>
#include <vector>
#include <list>

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

std::unordered_set<Symbol> takeUnion(const std::unordered_set<Symbol>& set1, const std::unordered_set<Symbol>& set2){
    std::unordered_set<Symbol> res = std::unordered_set<Symbol>();
    for (auto element : set1){
        res.emplace(element);
    }
    for (auto element : set2){
        res.emplace(element);
    }
    return res;
}

bool containsNull(const std::unordered_set<Symbol>& set);

std::unordered_set<Symbol> findFirst(std::list<Symbol> symbolList, std::vector<First*>* Firsts);