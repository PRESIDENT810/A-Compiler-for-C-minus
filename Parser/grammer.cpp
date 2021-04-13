#include "grammer.h"

bool isTerminal(Symbol symbol){
    return static_cast<int>(symbol) > nonTerminalCnt-1;
}

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

bool sameSymbol(Symbol symbol1, int n2){
    int n1 = static_cast<int>(symbol1);
    return n1 == n2;
}

//
// this function first construct the FIRST & FOLLOW set for each symbol,
// and then construct the LL(1) analysis table of all non-terminal symbols
//
LL1Table::LL1Table(std::vector<First*>* Firsts, std::vector<Follow*>* Follows, std::vector<Rule*>* rules){
    // initialize the table
    this->table = new Rule**[nonTerminalCnt];
    for (int i=0; i<nonTerminalCnt; i++) this->table[i] = new Rule*[terminalCnt];
    for (int i=0; i<nonTerminalCnt; i++){
        for (int j=0; j<terminalCnt; j++){
            this->table[i][j] = nullptr;
        }
    }
    // construct FIRST & FOLLOW set
    for (auto rule : *rules){
        Symbol nonterminal = rule->LHS;
        auto alpha = std::list<Symbol>();
        for (auto s : rule->RHS) alpha.emplace_back(s);
        auto firstSet = findFirst(alpha, Firsts);
        auto follow = (*Follows)[static_cast<int>(nonterminal)];
        auto followSet = follow->followSet;
        std::unordered_set<Symbol> lookaheads;

        if (!containsNull(firstSet)){
            lookaheads = firstSet;
        } else{
            lookaheads = takeUnion(firstSet, followSet);
        }
        for (auto s : lookaheads){
//            if (this->table[static_cast<int>(nonterminal)][static_cast<int>(s)-nonTerminalCnt] != nullptr && this->table[static_cast<int>(nonterminal)][static_cast<int>(s)-nonTerminalCnt] != rule){
//                printf("Fuck!\n");
//            }
            this->table[static_cast<int>(nonterminal)][static_cast<int>(s)-nonTerminalCnt] = rule;
        }
    }
}

bool containsNull(const std::unordered_set<Symbol>& set){
    for (auto s : set) if (static_cast<int>(s) == static_cast<int>(Symbol::nullStr)) return true;
    return false;
}

std::unordered_set<Symbol> findFirst(std::list<Symbol> symbolList, std::vector<First*>* Firsts){
    std::unordered_set<Symbol> res = std::unordered_set<Symbol>();

    if (symbolList.empty()){
        res.emplace(Symbol::nullStr);
        return res;
    }

    Symbol a1 = *symbolList.begin();
    First* Firsta1 = (*Firsts)[static_cast<int>(a1)];

    if (!Firsta1->containNull()){
        res = takeUnion(res, Firsta1->firstSet);
    } else{
        res = takeUnion(res, Firsta1->firstSet);
        res.erase(Symbol::nullStr);
        symbolList.pop_front();
        res = takeUnion(res, findFirst(symbolList, Firsts));
    }
    return res;
}

void makeFirsts(std::vector<First*>* Firsts, std::vector<Rule*>* rules){
    // compute initial guess
    for (int i=0; i<(nonTerminalCnt+terminalCnt); i++){
        auto symbol = static_cast<Symbol>(i);
        Firsts->push_back(new First(symbol));
    }
    // compute fixed point
    while (true){
        bool changed = false;
        for (Rule* rule : *rules){
            // some preparation
            Symbol symbol = rule->LHS;
            First* firstA = (*Firsts)[static_cast<int>(symbol)];
            int oldSize = firstA->firstSet.size();
            std::list<Symbol> symbolList = std::list<Symbol>();
            for (auto s : rule->RHS) symbolList.emplace_back(s);

            // union the value of First(A) with current guess of First
            std::unordered_set<Symbol> currentGuess = findFirst(symbolList, Firsts);
            firstA->takeUnion(currentGuess);

            int newSize = firstA->firstSet.size();
            if (oldSize != newSize) changed = true;
        }
        if (!changed) break;
    }
}

void makeFollows(std::vector<Follow*>* Follows, std::vector<First*>* Firsts, std::vector<Rule*>* rules){
    // compute initial guess
    for (int i=0; i<nonTerminalCnt; i++){
        auto symbol = static_cast<Symbol>(i);
        Follows->push_back(new Follow(symbol));
    }
    // compute fixed point
    while (true){
        bool changed = false;
        for (Rule* rule : *rules){
            // check whether this rule is in the form: A: alpha B beta where B is a non-terminal
            for (auto B : rule->RHS){
                if (!isTerminal(B)){ // s is the non-terminal symbol B
                    // some preparation
                    Symbol A = rule->LHS;
                    std::list<Symbol> beta = std::list<Symbol>();
                    for (auto iter=rule->RHS.end()-1; *iter!=B; iter--){
                        beta.emplace_front(*iter);
                    }
                    Follow* followB = (*Follows)[static_cast<int>(B)];
                    std::unordered_set<Symbol> firstBeta = findFirst(beta, Firsts);
                    int oldSize = followB->followSet.size();

                    // replace the value of Follow(B) with Follow(B) union First(beta) - nullStr
                    followB->takeUnion(firstBeta);
                    followB->followSet.erase(Symbol::nullStr);
                    // if nullStr in First(beta), set Follow(B) = Follow(B) union Follow(A)
                    if (containsNull(firstBeta)) followB->takeUnion((*Follows)[static_cast<int>(A)]->followSet);

                    int newSize = followB->followSet.size();
                    if (oldSize != newSize) changed = true;
                }
            }
        }
        if (!changed) break;
    }
}

void makeRules(std::vector<Rule*>* rules) {
    Rule* rule;
    // program: varDecls stmts
    rule = new Rule(Symbol::program);
    rule->RHS.push_back(Symbol::varDecls);
    rule->RHS.push_back(Symbol::stmts);
    rules->push_back(rule);
    // varDecls: varDecl varDecls
    rule = new Rule(Symbol::varDecls);
    rule->RHS.push_back(Symbol::varDecl);
    rule->RHS.push_back(Symbol::varDecls);
    rules->push_back(rule);
    // varDecls: nullStr
    rule = new Rule(Symbol::varDecls);
    rule->RHS.push_back(Symbol::nullStr);
    rules->push_back(rule);
    // varDecl: INT declList SEMI
    rule = new Rule(Symbol::varDecl);
    rule->RHS.push_back(Symbol::INT);
    rule->RHS.push_back(Symbol::declList);
    rule->RHS.push_back(Symbol::SEMI);
    rules->push_back(rule);
    // declList: decl declListPostfix
    rule = new Rule(Symbol::declList);
    rule->RHS.push_back(Symbol::decl);
    rule->RHS.push_back(Symbol::declListPostfix);
    rules->push_back(rule);
    // declListPostfix: COMMA declList
    rule = new Rule(Symbol::declListPostfix);
    rule->RHS.push_back(Symbol::COMMA);
    rule->RHS.push_back(Symbol::declList);
    rules->push_back(rule);
    // declListPostfix: nullStr
    rule = new Rule(Symbol::declListPostfix);
    rule->RHS.push_back(Symbol::nullStr);
    rules->push_back(rule);
    // decl: ID declPostfix
    rule = new Rule(Symbol::decl);
    rule->RHS.push_back(Symbol::ID);
    rule->RHS.push_back(Symbol::declPostfix);
    rules->push_back(rule);
    // declPostfix: ASSIGN INTNUM
    rule = new Rule(Symbol::declPostfix);
    rule->RHS.push_back(Symbol::ASSIGN);
    rule->RHS.push_back(Symbol::INTNUM);
    rules->push_back(rule);
    // declPostfix: LSQUARE INTNUM RSQUARE
    rule = new Rule(Symbol::declPostfix);
    rule->RHS.push_back(Symbol::LSQUARE);
    rule->RHS.push_back(Symbol::INTNUM);
    rule->RHS.push_back(Symbol::RSQUARE);
    rules->push_back(rule);
    // declPostfix: nullStr
    rule = new Rule(Symbol::declPostfix);
    rule->RHS.push_back(Symbol::nullStr);
    rules->push_back(rule);
    // codeBlock: stmt
    rule = new Rule(Symbol::codeBlock);
    rule->RHS.push_back(Symbol::stmt);
    rules->push_back(rule);
    // codeBlock: LBRACE stmts RBRACE
    rule = new Rule(Symbol::codeBlock);
    rule->RHS.push_back(Symbol::LBRACE);
    rule->RHS.push_back(Symbol::stmts);
    rule->RHS.push_back(Symbol::RBRACE);
    rules->push_back(rule);
    // stmts: stmt stmtsPostfix
    rule = new Rule(Symbol::stmts);
    rule->RHS.push_back(Symbol::stmt);
    rule->RHS.push_back(Symbol::stmtsPostfix);
    rules->push_back(rule);
    // stmtsPostfix: stmts
    rule = new Rule(Symbol::stmtsPostfix);
    rule->RHS.push_back(Symbol::stmts);
    rules->push_back(rule);
    // stmtsPostfix: nullStr
    rule = new Rule(Symbol::stmtsPostfix);
    rule->RHS.push_back(Symbol::nullStr);
    rules->push_back(rule);
    // stmt: assignStmt SEMI
    rule = new Rule(Symbol::stmt);
    rule->RHS.push_back(Symbol::assignStmt);
    rule->RHS.push_back(Symbol::SEMI);
    rules->push_back(rule);
    // stmt: controlStmt
    rule = new Rule(Symbol::stmt);
    rule->RHS.push_back(Symbol::controlStmt);
    rules->push_back(rule);
    // stmt: rwStmt SEMI
    rule = new Rule(Symbol::stmt);
    rule->RHS.push_back(Symbol::rwStmt);
    rule->RHS.push_back(Symbol::SEMI);
    rules->push_back(rule);
    // stmt: SEMI
    rule = new Rule(Symbol::stmt);
    rule->RHS.push_back(Symbol::SEMI);
    rules->push_back(rule);
    // controlStmt: ifStmts
    rule = new Rule(Symbol::controlStmt);
    rule->RHS.push_back(Symbol::ifStmts);
    rules->push_back(rule);
    // controlStmt: whileStmt
    rule = new Rule(Symbol::controlStmt);
    rule->RHS.push_back(Symbol::whileStmt);
    rules->push_back(rule);
    // controlStmt: dowhileStmt SEMI
    rule = new Rule(Symbol::controlStmt);
    rule->RHS.push_back(Symbol::dowhileStmt);
    rule->RHS.push_back(Symbol::SEMI);
    rules->push_back(rule);
    // controlStmt: returnStmt SEMI
    rule = new Rule(Symbol::controlStmt);
    rule->RHS.push_back(Symbol::returnStmt);
    rule->RHS.push_back(Symbol::SEMI);
    rules->push_back(rule);
    // rwStmt: rStmt
    rule = new Rule(Symbol::rwStmt);
    rule->RHS.push_back(Symbol::rStmt);
    rules->push_back(rule);
    // rwStmt: wStmt
    rule = new Rule(Symbol::rwStmt);
    rule->RHS.push_back(Symbol::wStmt);
    rules->push_back(rule);
    // assignStmt: ID assignStmtPostfix
    rule = new Rule(Symbol::assignStmt);
    rule->RHS.push_back(Symbol::ID);
    rule->RHS.push_back(Symbol::assignStmtPostfix);
    rules->push_back(rule);
    // assignStmtPostfix: LSQUARE exp RSQUARE ASSIGN exp
    rule = new Rule(Symbol::assignStmtPostfix);
    rule->RHS.push_back(Symbol::LSQUARE);
    rule->RHS.push_back(Symbol::exp);
    rule->RHS.push_back(Symbol::RSQUARE);
    rule->RHS.push_back(Symbol::ASSIGN);
    rule->RHS.push_back(Symbol::exp);
    rules->push_back(rule);
    // assignStmtPostfix: ASSIGN exp
    rule = new Rule(Symbol::assignStmtPostfix);
    rule->RHS.push_back(Symbol::ASSIGN);
    rule->RHS.push_back(Symbol::exp);
    rules->push_back(rule);
    // ifStmts: ifStmt ifStmtsPostfix
    rule = new Rule(Symbol::ifStmts);
    rule->RHS.push_back(Symbol::ifStmt);
    rule->RHS.push_back(Symbol::ifStmtsPostfix);
    rules->push_back(rule);
    // ifStmtsPostfix: nullStr
    rule = new Rule(Symbol::ifStmtsPostfix);
    rule->RHS.push_back(Symbol::nullStr);
    rules->push_back(rule);
    // ifStmtsPostfix: ELSE codeBlock
    rule = new Rule(Symbol::ifStmtsPostfix);
    rule->RHS.push_back(Symbol::ELSE);
    rule->RHS.push_back(Symbol::codeBlock);
    rules->push_back(rule);
    // ifStmt: IF LPAR exp RPAR codeBlock
    rule = new Rule(Symbol::ifStmt);
    rule->RHS.push_back(Symbol::IF);
    rule->RHS.push_back(Symbol::LPAR);
    rule->RHS.push_back(Symbol::exp);
    rule->RHS.push_back(Symbol::RPAR);
    rule->RHS.push_back(Symbol::codeBlock);
    rules->push_back(rule);
    // whileStmt: WHILE LPAR exp RPAR codeBlock
    rule = new Rule(Symbol::whileStmt);
    rule->RHS.push_back(Symbol::WHILE);
    rule->RHS.push_back(Symbol::LPAR);
    rule->RHS.push_back(Symbol::exp);
    rule->RHS.push_back(Symbol::RPAR);
    rule->RHS.push_back(Symbol::codeBlock);
    rules->push_back(rule);
    // dowhileStmt: DO codeBlock WHILE LPAR exp RPAR
    rule = new Rule(Symbol::dowhileStmt);
    rule->RHS.push_back(Symbol::DO);
    rule->RHS.push_back(Symbol::codeBlock);
    rule->RHS.push_back(Symbol::WHILE);
    rule->RHS.push_back(Symbol::LPAR);
    rule->RHS.push_back(Symbol::exp);
    rule->RHS.push_back(Symbol::RPAR);
    rules->push_back(rule);
    // returnStmt: RETURN
    rule = new Rule(Symbol::returnStmt);
    rule->RHS.push_back(Symbol::RETURN);
    rules->push_back(rule);
    // rStmt: READ LPAR ID RPAR
    rule = new Rule(Symbol::rStmt);
    rule->RHS.push_back(Symbol::READ);
    rule->RHS.push_back(Symbol::LPAR);
    rule->RHS.push_back(Symbol::ID);
    rule->RHS.push_back(Symbol::RPAR);
    rules->push_back(rule);
    // wStmt: WRITE LPAR exp RPAR
    rule = new Rule(Symbol::wStmt);
    rule->RHS.push_back(Symbol::WRITE);
    rule->RHS.push_back(Symbol::LPAR);
    rule->RHS.push_back(Symbol::exp);
    rule->RHS.push_back(Symbol::RPAR);
    rules->push_back(rule);
    // exp: realExp OpExp
    rule = new Rule(Symbol::exp);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // realExp: ID realExpPostfix
    rule = new Rule(Symbol::realExp);
    rule->RHS.push_back(Symbol::ID);
    rule->RHS.push_back(Symbol::realExpPostfix);
    rules->push_back(rule);
    // realExpPostfix: nullStr
    rule = new Rule(Symbol::realExpPostfix);
    rule->RHS.push_back(Symbol::nullStr);
    rules->push_back(rule);
    // realExpPostfix: LSQUARE exp RSQUARE
    rule = new Rule(Symbol::realExpPostfix);
    rule->RHS.push_back(Symbol::LSQUARE);
    rule->RHS.push_back(Symbol::exp);
    rule->RHS.push_back(Symbol::RSQUARE);
    rules->push_back(rule);
    // realExp: INTNUM
    rule = new Rule(Symbol::realExp);
    rule->RHS.push_back(Symbol::INTNUM);
    rules->push_back(rule);
    // realExp: NOT_OP exp
    rule = new Rule(Symbol::realExp);
    rule->RHS.push_back(Symbol::NOT_OP);
    rule->RHS.push_back(Symbol::exp);
    rules->push_back(rule);
    // realExp: LPAR exp RPAR
    rule = new Rule(Symbol::realExp);
    rule->RHS.push_back(Symbol::LPAR);
    rule->RHS.push_back(Symbol::exp);
    rule->RHS.push_back(Symbol::RPAR);
    rules->push_back(rule);
    // realExp: MINUS exp
    rule = new Rule(Symbol::realExp);
    rule->RHS.push_back(Symbol::MINUS);
    rule->RHS.push_back(Symbol::exp);
    rules->push_back(rule);
    // OpExp: nullStr
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::nullStr);
    rules->push_back(rule);
    // OpExp: AND_OP realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::AND_OP);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: OR_OP realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::OR_OP);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: PLUS realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::PLUS);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: MINUS realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::MINUS);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: MUL_OP realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::MUL_OP);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: DIV_OP realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::DIV_OP);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: LT realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::LT);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: GT realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::GT);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: EQ realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::EQ);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: NOTEQ realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::NOTEQ);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: LTEQ realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::LTEQ);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: GTEQ realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::GTEQ);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: SHL_OP realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::SHL_OP);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: SHR_OP realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::SHR_OP);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: ANDAND realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::ANDAND);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
    // OpExp: OROR realExp OpExp
    rule = new Rule(Symbol::OpExp);
    rule->RHS.push_back(Symbol::OROR);
    rule->RHS.push_back(Symbol::realExp);
    rule->RHS.push_back(Symbol::OpExp);
    rules->push_back(rule);
}