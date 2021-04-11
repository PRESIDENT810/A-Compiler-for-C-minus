#include "grammer.h"

//
// this function first construct the FIRST & FOLLOW set for each symbol,
// and then construct the LL(1) analysis table of all non-terminal symbols
//
LL1Table::LL1Table() {
    // initialize the table
    this->table = new int*[19];
    for (int i=0; i<19; i++) this->table[i] = new int[39];
    // make all production rules
    makeRules();
    // construct FIRST & FOLLOW set
    makeFirsts();
    makeFollows();
}

//
// this function find the correct rule to be applied represented by an int
// the input is the non-terminal symbol and the next non-null lookahead symbol
//
int LL1Table::findProduction(Symbol nonterminal, Symbol lookahead) {
    return this->table[static_cast<int>(nonterminal)][static_cast<int>(lookahead)-18];
}

std::unordered_set<Symbol> findFirst(std::deque<Symbol> symbolVec){
    auto res = std::unordered_set<Symbol>();

    Symbol x1 = symbolVec[0];
    auto Firstx1 = Firsts[static_cast<int>(x1)];

    if (symbolVec.empty()){
        res.emplace(Symbol::nullStr);
    } else if (!Firstx1->containNull()){
        res.merge(Firstx1->firstSet);
    } else{
        res.merge(Firstx1->firstSet);
        res.erase(Symbol::nullStr);
        symbolVec.pop_front();
        res.merge(findFirst(symbolVec));
    }
    return res;
}

void makeFirsts(){
    // compute initial guess
    for (int i=0; i<60; i++){
        auto symbol = static_cast<Symbol>(i);
        Firsts.emplace_back(new First(symbol));
    }
    // compute fixed point
    while (true){
        bool changed = false;
        for (auto rule : rules){
            Symbol symbol = rule->LHS;
            auto firstA = Firsts[static_cast<int>(symbol)];
            // union the value of First(A) with current guess of First
            int oldSize = firstA->firstSet.size();
            auto currentGuess = findFirst(rule->RHS);
            firstA->update(currentGuess);
            int newSize = firstA->firstSet.size();
            if (oldSize != newSize) changed = true;
        }
        if (!changed) break;
    }
}

void makeFollows(){

}

void makeRules() {
    Rule* rule;
    // program: varDeclarations stmts
    rule = new Rule(Symbol::program);
    rule->RHS.emplace_back(Symbol::varDecl);
    rule->RHS.emplace_back(Symbol::stmts);
    rules.emplace_back(rule);
    // varDeclarations: varDeclaration varDeclarations
    rule = new Rule(Symbol::varDecls);
    rule->RHS.emplace_back(Symbol::varDecl);
    rule->RHS.emplace_back(Symbol::varDecls);
    rules.emplace_back(rule);
    // varDeclarations: null
    rule = new Rule(Symbol::varDecls);
    rule->RHS.emplace_back(Symbol::nullStr);
    rules.emplace_back(rule);
    // varDeclaration: INT declarationList SEMI
    rule = new Rule(Symbol::varDecl);
    rule->RHS.emplace_back(Symbol::INT);
    rule->RHS.emplace_back(Symbol::declList);
    rule->RHS.emplace_back(Symbol::SEMI);
    rules.emplace_back(rule);
    // declarationList: declaration COMMA declarationList
    rule = new Rule(Symbol::declList);
    rule->RHS.emplace_back(Symbol::decl);
    rule->RHS.emplace_back(Symbol::COMMA);
    rule->RHS.emplace_back(Symbol::declList);
    rules.emplace_back(rule);
    // declarationList: declaration
    rule = new Rule(Symbol::declList);
    rule->RHS.emplace_back(Symbol::decl);
    rules.emplace_back(rule);
    // declaration: ID ASSIGN INTNUM
    rule = new Rule(Symbol::decl);
    rule->RHS.emplace_back(Symbol::ID);
    rule->RHS.emplace_back(Symbol::ASSIGN);
    rule->RHS.emplace_back(Symbol::INTNUM);
    rules.emplace_back(rule);
    // declaration: ID LSQUARE INTNUM RSQUARE
    rule = new Rule(Symbol::decl);
    rule->RHS.emplace_back(Symbol::ID);
    rule->RHS.emplace_back(Symbol::LSQUARE);
    rule->RHS.emplace_back(Symbol::INTNUM);
    rule->RHS.emplace_back(Symbol::RSQUARE);
    rules.emplace_back(rule);
    // declaration: ID
    rule = new Rule(Symbol::decl);
    rule->RHS.emplace_back(Symbol::ID);
    rules.emplace_back(rule);
    // codeBlock: statement
    rule = new Rule(Symbol::codeBlock);
    rule->RHS.emplace_back(Symbol::stmt);
    rules.emplace_back(rule);
    // codeBlock: LBRACE stmts RBRACE
    rule = new Rule(Symbol::codeBlock);
    rule->RHS.emplace_back(Symbol::LBRACE);
    rule->RHS.emplace_back(Symbol::stmts);
    rule->RHS.emplace_back(Symbol::RBRACE);
    rules.emplace_back(rule);
    // stmts: stmt stmts
    rule = new Rule(Symbol::stmts);
    rule->RHS.emplace_back(Symbol::stmt);
    rule->RHS.emplace_back(Symbol::stmts);
    rules.emplace_back(rule);
    // stmts: stmt
    rule = new Rule(Symbol::stmts);
    rule->RHS.emplace_back(Symbol::stmt);
    rules.emplace_back(rule);
    // stmt: assignStmt SEMI
    rule = new Rule(Symbol::stmt);
    rule->RHS.emplace_back(Symbol::assignStmt);
    rule->RHS.emplace_back(Symbol::SEMI);
    rules.emplace_back(rule);
    // stmt: controlStmt
    rule = new Rule(Symbol::stmt);
    rule->RHS.emplace_back(Symbol::controlStmt);
    rules.emplace_back(rule);
    // stmt: rwStmt SEMI
    rule = new Rule(Symbol::stmt);
    rule->RHS.emplace_back(Symbol::rwStmt);
    rule->RHS.emplace_back(Symbol::SEMI);
    rules.emplace_back(rule);
    // stmt: SEMI
    rule = new Rule(Symbol::stmt);
    rule->RHS.emplace_back(Symbol::SEMI);
    rules.emplace_back(rule);
    // controlStmt: ifStmts
    rule = new Rule(Symbol::controlStmt);
    rule->RHS.emplace_back(Symbol::ifStmts);
    rules.emplace_back(rule);
    // controlStmt: whileStmt
    rule = new Rule(Symbol::controlStmt);
    rule->RHS.emplace_back(Symbol::whileStmt);
    rules.emplace_back(rule);
    // controlStmt: dowhileStmt SEMI
    rule = new Rule(Symbol::dowhileStmt);
    rule->RHS.emplace_back(Symbol::SEMI);
    rules.emplace_back(rule);
    // controlStmt: returnStmt SEMI
    rule = new Rule(Symbol::returnStmt);
    rule->RHS.emplace_back(Symbol::SEMI);
    rules.emplace_back(rule);
    // rwStmt: rStmt
    rule = new Rule(Symbol::rwStmt);
    rule->RHS.emplace_back(Symbol::rStmt);
    rules.emplace_back(rule);
    // rwStmt: wStmt
    rule = new Rule(Symbol::rwStmt);
    rule->RHS.emplace_back(Symbol::wStmt);
    rules.emplace_back(rule);
    // assignStmt: ID LSQUARE exp RSQUARE ASSIGN exp
    rule = new Rule(Symbol::assignStmt);
    rule->RHS.emplace_back(Symbol::ID);
    rule->RHS.emplace_back(Symbol::LSQUARE);
    rule->RHS.emplace_back(Symbol::exp);
    rule->RHS.emplace_back(Symbol::RSQUARE);
    rule->RHS.emplace_back(Symbol::ASSIGN);
    rule->RHS.emplace_back(Symbol::exp);
    rules.emplace_back(rule);
    // assignStmt: ID ASSIGN exp
    rule = new Rule(Symbol::assignStmt);
    rule->RHS.emplace_back(Symbol::ID);
    rule->RHS.emplace_back(Symbol::ASSIGN);
    rule->RHS.emplace_back(Symbol::exp);
    rules.emplace_back(rule);
    // ifStmts: ifStmt
    rule = new Rule(Symbol::ifStmts);
    rule->RHS.emplace_back(Symbol::ifStmt);
    rules.emplace_back(rule);
    // ifStmts: ifStmt ELSE codeBlock
    rule = new Rule(Symbol::ifStmts);
    rule->RHS.emplace_back(Symbol::ifStmt);
    rule->RHS.emplace_back(Symbol::ELSE);
    rule->RHS.emplace_back(Symbol::codeBlock);
    rules.emplace_back(rule);
    // ifStmt: IF LPAR exp RPAR codeBlock
    rule = new Rule(Symbol::ifStmt);
    rule->RHS.emplace_back(Symbol::IF);
    rule->RHS.emplace_back(Symbol::LPAR);
    rule->RHS.emplace_back(Symbol::exp);
    rule->RHS.emplace_back(Symbol::RPAR);
    rule->RHS.emplace_back(Symbol::codeBlock);
    rules.emplace_back(rule);
    // whileStmt: WHILE LPAR exp RPAR codeBlock
    rule = new Rule(Symbol::whileStmt);
    rule->RHS.emplace_back(Symbol::WHILE);
    rule->RHS.emplace_back(Symbol::LPAR);
    rule->RHS.emplace_back(Symbol::exp);
    rule->RHS.emplace_back(Symbol::RPAR);
    rule->RHS.emplace_back(Symbol::codeBlock);
    rules.emplace_back(rule);
    // dowhileStmt: DO codeBlock WHILE LPAR exp RPAR
    rule = new Rule(Symbol::dowhileStmt);
    rule->RHS.emplace_back(Symbol::DO);
    rule->RHS.emplace_back(Symbol::codeBlock);
    rule->RHS.emplace_back(Symbol::WHILE);
    rule->RHS.emplace_back(Symbol::LPAR);
    rule->RHS.emplace_back(Symbol::exp);
    rule->RHS.emplace_back(Symbol::RPAR);
    rules.emplace_back(rule);
    // returnStmt: RETURN
    rule = new Rule(Symbol::returnStmt);
    rule->RHS.emplace_back(Symbol::RETURN);
    rules.emplace_back(rule);
    // rStmt: READ LPAR ID RPAR
    rule = new Rule(Symbol::rStmt);
    rule->RHS.emplace_back(Symbol::READ);
    rule->RHS.emplace_back(Symbol::LPAR);
    rule->RHS.emplace_back(Symbol::ID);
    rule->RHS.emplace_back(Symbol::RPAR);
    rules.emplace_back(rule);
    // wStmt: WRITE LPAR exp RPAR
    rule = new Rule(Symbol::wStmt);
    rule->RHS.emplace_back(Symbol::WRITE);
    rule->RHS.emplace_back(Symbol::LPAR);
    rule->RHS.emplace_back(Symbol::exp);
    rule->RHS.emplace_back(Symbol::RPAR);
    rules.emplace_back(rule);
    // exp: realExp OpExp
    rule = new Rule(Symbol::exp);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // realExp: ID
    rule = new Rule(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::ID);
    rules.emplace_back(rule);
    // realExp: ID LSQUARE exp RSQUARE
    rule = new Rule(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::ID);
    rule->RHS.emplace_back(Symbol::LSQUARE);
    rule->RHS.emplace_back(Symbol::exp);
    rule->RHS.emplace_back(Symbol::RSQUARE);
    rules.emplace_back(rule);
    // realExp: NOT_OP exp
    rule = new Rule(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::NOT_OP);
    rule->RHS.emplace_back(Symbol::exp);
    rules.emplace_back(rule);
    // realExp: LPAR exp RPAR
    rule = new Rule(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::LPAR);
    rule->RHS.emplace_back(Symbol::exp);
    rule->RHS.emplace_back(Symbol::RPAR);
    rules.emplace_back(rule);
    // realExp: MINUS exp
    rule = new Rule(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::MINUS);
    rule->RHS.emplace_back(Symbol::exp);
    rules.emplace_back(rule);
    // OpExp: null
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(nullptr);
    rules.emplace_back(rule);
    // OpExp: AND_OP realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::AND_OP);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: OR_OP realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::OR_OP);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: PLUS realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::PLUS);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: MINUS realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::MINUS);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: MUL_OP realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::MUL_OP);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: DIV_OP realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::DIV_OP);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: LT realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::LT);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: GT realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::GT);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: EQ realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::EQ);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: NOTEQ realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::NOTEQ);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: LTEQ realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::LTEQ);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: GTEQ realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::GTEQ);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: SHL_OP realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::SHL_OP);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: SHR_OP realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::SHR_OP);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: ANDAND realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::ANDAND);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
    // OpExp: OROR realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.emplace_back(Symbol::OROR);
    rule->RHS.emplace_back(Symbol::realExp);
    rule->RHS.emplace_back(Symbol::OpExp);
    rules.emplace_back(rule);
}