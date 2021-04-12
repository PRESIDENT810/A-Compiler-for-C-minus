#include "Parse.h"

std::vector<Rule*> rules;
std::vector<First*> Firsts;
std::vector<Follow*> Follows;
LL1Table* table;

Symbol inferType(char* tokenValue){
    // non-terminal symbols
    if (strcmp(tokenValue, "program") == 0) return Symbol::program;
    if (strcmp(tokenValue, "varDecls") == 0) return Symbol::varDecls;
    if (strcmp(tokenValue, "varDecl") == 0) return Symbol::varDecl;
    if (strcmp(tokenValue, "declList") == 0) return Symbol::declList;
    if (strcmp(tokenValue, "decl") == 0) return Symbol::decl;
    if (strcmp(tokenValue, "codeBlock") == 0) return Symbol::codeBlock;
    if (strcmp(tokenValue, "stmts") == 0) return Symbol::stmts;
    if (strcmp(tokenValue, "stmt") == 0) return Symbol::stmt;
    if (strcmp(tokenValue, "controlStmt") == 0) return Symbol::controlStmt;
    if (strcmp(tokenValue, "rwStmt") == 0) return Symbol::rwStmt;
    if (strcmp(tokenValue, "assignStmt") == 0) return Symbol::assignStmt;
    if (strcmp(tokenValue, "ifStmts") == 0) return Symbol::ifStmts;
    if (strcmp(tokenValue, "ifStmt") == 0) return Symbol::ifStmt;
    if (strcmp(tokenValue, "whileStmt") == 0) return Symbol::whileStmt;
    if (strcmp(tokenValue, "dowhileStmt") == 0) return Symbol::dowhileStmt;
    if (strcmp(tokenValue, "returnStmt") == 0) return Symbol::returnStmt;
    if (strcmp(tokenValue, "rStmt") == 0) return Symbol::rStmt;
    if (strcmp(tokenValue, "wStmt") == 0) return Symbol::wStmt;
    if (strcmp(tokenValue, "exp") == 0) return Symbol::exp;
    if (strcmp(tokenValue, "OpExp") == 0) return Symbol::OpExp;
    // reversed words
    if (strcmp(tokenValue, "INT") == 0) return Symbol::INT;
    if (strcmp(tokenValue, "MAIN") == 0) return Symbol::MAIN;
    if (strcmp(tokenValue, "VOID") == 0) return Symbol::VOID;
    if (strcmp(tokenValue, "BREAK") == 0) return Symbol::BREAK;
    if (strcmp(tokenValue, "DO") == 0) return Symbol::DO;
    if (strcmp(tokenValue, "ELSE") == 0) return Symbol::ELSE;
    if (strcmp(tokenValue, "IF") == 0) return Symbol::IF;
    if (strcmp(tokenValue, "WHILE") == 0) return Symbol::WHILE;
    if (strcmp(tokenValue, "RETURN") == 0) return Symbol::RETURN;
    if (strcmp(tokenValue, "READ") == 0) return Symbol::READ;
    if (strcmp(tokenValue, "WRITE") == 0) return Symbol::WRITE;
    // special symbols
    if (strcmp(tokenValue, "LBRACE") == 0) return Symbol::LBRACE;
    if (strcmp(tokenValue, "RBRACE") == 0) return Symbol::RBRACE;
    if (strcmp(tokenValue, "LSQUARE") == 0) return Symbol::LSQUARE;
    if (strcmp(tokenValue, "RSQUARE") == 0) return Symbol::RSQUARE;
    if (strcmp(tokenValue, "LPAR") == 0) return Symbol::LPAR;
    if (strcmp(tokenValue, "RPAR") == 0) return Symbol::RPAR;
    if (strcmp(tokenValue, "SEMI") == 0) return Symbol::SEMI;
    if (strcmp(tokenValue, "PLUS") == 0) return Symbol::PLUS;
    if (strcmp(tokenValue, "MINUS") == 0) return Symbol::MINUS;
    if (strcmp(tokenValue, "MUL_OP") == 0) return Symbol::MUL_OP;
    if (strcmp(tokenValue, "DIV_OP") == 0) return Symbol::DIV_OP;
    if (strcmp(tokenValue, "AND_OP") == 0) return Symbol::AND_OP;
    if (strcmp(tokenValue, "OR_OP") == 0) return Symbol::OR_OP;
    if (strcmp(tokenValue, "NOT_OP") == 0) return Symbol::NOT_OP;
    if (strcmp(tokenValue, "ASSIGN") == 0) return Symbol::ASSIGN;
    if (strcmp(tokenValue, "LT") == 0) return Symbol::LT;
    if (strcmp(tokenValue, "GT") == 0) return Symbol::GT;
    if (strcmp(tokenValue, "SHL_OP") == 0) return Symbol::SHL_OP;
    if (strcmp(tokenValue, "SHR_OP") == 0) return Symbol::SHR_OP;
    if (strcmp(tokenValue, "EQ") == 0) return Symbol::EQ;
    if (strcmp(tokenValue, "NOTEQ") == 0) return Symbol::NOTEQ;
    if (strcmp(tokenValue, "LTEQ") == 0) return Symbol::LTEQ;
    if (strcmp(tokenValue, "GTEQ") == 0) return Symbol::GTEQ;
    if (strcmp(tokenValue, "ANDAND") == 0) return Symbol::ANDAND;
    if (strcmp(tokenValue, "OROR") == 0) return Symbol::OROR;
    if (strcmp(tokenValue, "COMMA") == 0) return Symbol::COMMA;
    // regex symbols
    if (strcmp(tokenValue, "INTNUM") == 0) return Symbol::INTNUM;
    if (strcmp(tokenValue, "ID") == 0) return Symbol::ID;
    // null string
    if (strcmp(tokenValue, "nullStr") == 0) return Symbol::nullStr;
    return Symbol::nullStr;
}

std::list<Token*> tokenScanner(){
    std::list<Token*> res = std::list<Token*>();

    char filename[30] = "../ScannerOutput.txt";
    FILE* fp = nullptr;
    fp = fopen(filename, "r");
    char* semanticBuffer = new char[20];
    char* tokenBuffer = new char[20];
    int N = 99;
    while(fgets(semanticBuffer, N, fp) != nullptr){
        fgets(tokenBuffer, N, fp);
        semanticBuffer[strlen(semanticBuffer)-1] = '\0';
        tokenBuffer[strlen(tokenBuffer)-1] = '\0';
        res.emplace_back(new Token(semanticBuffer, tokenBuffer));
    }
    return res;
}

inline Rule* matchRule(Symbol crtSymbol, Token* lookahead){
    return table->findProduction(crtSymbol, lookahead->type);
}

TreeNode* recursiveParse(Symbol crtSymbol, std::list<Token*>::iterator iter, TreeNode* parentNode){
    auto potentialRules = std::list<Rule*>();
    Rule* appliedRule;

    // find the correct rule to apply
    for (auto rule : rules){
        if (rule->LHS == crtSymbol) potentialRules.emplace_back(rule);
    }
    if (potentialRules.size() > 1){ // we need to find the correct rule using the table
        iter++; // peek the next token
        Token* lookahead = *iter; // get the token
        appliedRule = matchRule(crtSymbol, lookahead);
    } else{
        appliedRule = *potentialRules.begin();
    }
    // iterate the RHS of the rule and build TreeNode recursively
    TreeNode* crtNode = new TreeNode(crtSymbol, parentNode);
    for (auto symbol : appliedRule->RHS){
        TreeNode* childNode = recursiveParse(symbol, iter, crtNode);
        crtNode->myChildren.emplace_back();
    }
    return crtNode;
}

int main(){
    makeRules(&rules);
    makeFirsts(&Firsts, &rules);
    makeFollows(&Follows, &Firsts, &rules);
    table = new LL1Table(Firsts, Follows, rules);
    std::list<Token*> tokenList = tokenScanner();
    TreeNode* root = recursiveParse(Symbol::program, tokenList.begin(), nullptr);
    // TODO: change the grammar so we can fuck left common prefix
    return 0;
}