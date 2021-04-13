#include "Parse.h"
#include <exception>

std::vector<Rule*> rules;
std::vector<First*> Firsts;
std::vector<Follow*> Follows;
LL1Table* table;

static int iter = 0;

char symbolToStr[67][20] = {
        // non-terminal symbols
        "program",
        "varDecls",
        "varDecl",
        "declList",
        "declListPostfix",
        "decl",
        "declPostfix",
        "codeBlock",
        "stmts",
        "stmtsPostfix",
        "stmt",
        "controlStmt",
        "rwStmt",
        "assignStmt",
        "assignStmtPostfix",
        "ifStmts",
        "ifStmtsPostfix",
        "ifStmt",
        "whileStmt",
        "dowhileStmt",
        "returnStmt",
        "rStmt",
        "wStmt",
        "exp",
        "realExp",
        "realExpPostfix",
        "OpExp",
        // reversed words
        "INT",
        "MAIN",
        "VOID",
        "BREAK",
        "DO",
        "ELSE",
        "IF",
        "WHILE",
        "RETURN",
        "READ",
        "WRITE",
        // special symbols
        "LBRACE",
        "RBRACE",
        "LSQUARE",
        "RSQUARE",
        "LPAR",
        "RPAR",
        "SEMI",
        "PLUS",
        "MINUS",
        "MUL_OP",
        "DIV_OP",
        "AND_OP",
        "OR_OP",
        "NOT_OP",
        "ASSIGN",
        "LT",
        "GT",
        "SHL_OP",
        "SHR_OP",
        "EQ",
        "NOTEQ",
        "LTEQ",
        "GTEQ",
        "ANDAND",
        "OROR",
        "COMMA",
        // regex symbols
        "INTNUM",
        "ID",
        // null string
        "nullStr",
};

Symbol inferType(char* tokenValue) {
    for (int i=0; i<nonTerminalCnt+terminalCnt; i++){
        if (strcmp(tokenValue, symbolToStr[i]) == 0){
            return static_cast<Symbol>(i);
        }
    }
    return Symbol::nullStr;
}

std::vector<Token*> tokenScanner(char* filename){
    std::vector<Token*> res = std::vector<Token*>();

    FILE* fp = nullptr;
    fp = fopen(filename, "r");
    if (fp == nullptr) throw std::runtime_error("No valid file!");
        
    char* semanticBuffer = new char[20];
    char* tokenBuffer = new char[20];
    int N = 99;
    while(fgets(semanticBuffer, N, fp) != nullptr){
        fgets(tokenBuffer, N, fp);
        semanticBuffer[strlen(semanticBuffer)-1] = '\0';
        tokenBuffer[strlen(tokenBuffer)-1] = '\0';
        res.emplace_back(new Token(semanticBuffer, tokenBuffer));
    }
    res.emplace_back(new Token());
    return res;
}

inline Rule* matchRule(Symbol crtSymbol, Token* lookahead){
    auto res = table->findProduction(crtSymbol, lookahead->type);
    return res == nullptr ? table->findProduction(crtSymbol, Symbol::nullStr) : res;
}

void printNode(TreeNode* crtNode){
    // print indention
    for (int i=0; i<crtNode->depth; i++){
        printf("    ");
    }
    // print symbol type
    printf("--[%s]\n", symbolToStr[static_cast<int>(crtNode->mySymbol)]);
}

Rule* chooseIfRule(std::vector<Token*> tokenVec, TreeNode* crtNode){
    int tempIter = iter;
    for (int i=tempIter; i<tokenVec.size(); i++){
        if (tokenVec[i]->type == Symbol::IF){
            // if find IF first, then use rule: ifStmtsPostfix->nullStr
            return rules[31];
        } else if (tokenVec[i]->type == Symbol::ELSE){
            // if find ELSE first, then use rule: ifStmtsPostfix->ELSE codeBlock
            return rules[32];
        }
    }
    return rules[31];
}

TreeNode* recursiveParse(Symbol crtSymbol, std::vector<Token*> tokenVec, TreeNode* parentNode, int depth){
    auto* crtNode = new TreeNode(crtSymbol, parentNode, depth);
    // base case: if the symbol is a terminal, then return
    if (isTerminal(crtSymbol)) {
        // when current node is a terminal, advance your iterator to the next token
        if (crtSymbol != Symbol::nullStr) iter++;
        return crtNode;
    }

    auto potentialRules = std::list<Rule*>();
    Rule* appliedRule;
    Token* lookahead;

    // find the correct rule to apply
    if (crtSymbol == Symbol::ifStmtsPostfix){
        appliedRule = chooseIfRule(tokenVec, crtNode);
    }
    // TODO: solve precedence and associativity here
    else if (crtSymbol == Symbol::OpExp && false){
//        appliedRule = chooseOpExpRule(tokenVec, crtNode);
    }
    else{
        for (auto rule : rules){
            if (rule->LHS == crtSymbol) potentialRules.emplace_back(rule);
        }
        if (potentialRules.size() > 1){ // we need to find the correct rule using the table
            lookahead = tokenVec[iter]; // get the token
            appliedRule = matchRule(crtSymbol, lookahead);
        } else{
            appliedRule = *potentialRules.begin();
        }
    }

    // check whether we have an applied rule
    if (appliedRule == nullptr){
        throw std::runtime_error("No rule matched!\n");
    }

    // iterate the RHS of the rule and build TreeNode recursively
    for (auto symbol : appliedRule->RHS){
        TreeNode* childNode = recursiveParse(symbol, tokenVec, crtNode, depth+1);
        crtNode->myChildren.emplace_back(childNode);
    }
    return crtNode;
}

void showTree(TreeNode* crtNode){
    printNode(crtNode);
    for (auto node : crtNode->myChildren) showTree(node);
}

int main(){
    makeRules(&rules);
    makeFirsts(&Firsts, &rules);
    makeFollows(&Follows, &Firsts, &rules);
    table = new LL1Table(&Firsts, &Follows, &rules);
    char filename[30] = "./ScannerOutput.txt";
    std::vector<Token*> tokenVec = tokenScanner(filename);
    // TODO: special handler for if
    // TODO: special handler for OpExp
    // TODO: special handler for precedence
    try {
        TreeNode* root = recursiveParse(Symbol::program, tokenVec, nullptr, 0);
        showTree(root);
    } catch (std::exception &e){
        printf("%s", e.what());
        printf("Wrong grammar, cannot parse!\n");
    }
    return 0;
}