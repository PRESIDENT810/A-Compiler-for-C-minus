#include "symbol.h"
#include <unordered_set>
#include <deque>

class Rule{
public:
    Symbol LHS;
    std::deque<Symbol> RHS;
    Rule(Symbol symbol){
        this->LHS = symbol;
        this->RHS = std::deque<Symbol>();
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
    void update(std::unordered_set<Symbol> currentGuess){ // union the value of firstSet with the current guess
        this->firstSet.merge(currentGuess);
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
};

std::deque<Rule*> rules;
std::deque<First*> Firsts;
std::deque<Follow*> Follows;

class LL1Table{
public:
    int** table; // the transition table for LL(1) parser
    LL1Table();
    int findProduction(Symbol nonterminal, Symbol lookahead);
};

void makeFirsts();

void makeFollows();

void makeRules();