#include "regex.h"

NFA* reservedGen(const char* reservedWord, const char* token){
    auto vec = std::vector<NFA*>();
    for (int i=0; i<strlen(reservedWord); i++){
        vec.emplace_back(new NFA(reservedWord[i]));
    }
    NFA* res = catNFA(vec);

    res->endNode->token = new char[10];
    strcpy(res->endNode->token, token);
    return res;
}

NFA* symbolGen(const char* symbol, const char* token){
    auto vec = std::vector<NFA*>();
    for (int i=0; i<strlen(symbol); i++){
        vec.emplace_back(new NFA(symbol[i]));
    }
    NFA* res = catNFA(vec);

    res->endNode->token = new char[10];
    strcpy(res->endNode->token, token);
    return res;
}

NFA* digitGen(){
    char s[11] = "0123456789";
    return new NFA(s);
}

NFA* letterGen(){
    char s[53] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    return new NFA(s);
}

NFA* intnumGen(){
    auto vec = std::vector<NFA*>();
    NFA* digitNFA = digitGen();
    NFA* multi_digitNFA = reptNFA(digitGen());
    // generate NFA for regex: digit+
    vec.emplace_back(digitNFA);
    vec.emplace_back(multi_digitNFA);
    NFA* digitPlusNFA = catNFA(vec);
    // generate NFA for regex: +|-
    vec.clear();
    vec.emplace_back(new NFA('+'));
    vec.emplace_back(new NFA('-'));
    NFA* signNFA = orNFA(vec);
    // generate NFA for regex: (+|-)?
    vec.clear();
    vec.emplace_back(new NFA());
    vec.emplace_back(signNFA);
    NFA* signOptionalNFA = orNFA(vec);
    // generate NFA for regex: (+|-)?.digit+
    vec.clear();
    vec.emplace_back(signOptionalNFA);
    vec.emplace_back(digitPlusNFA);
    NFA* res = catNFA(vec);

    res->endNode->token = new char[10];
    strcpy(res->endNode->token, "INTNUM");
    return res;
}

NFA* idGen(){
    auto vec = std::vector<NFA*>();
    NFA* letterNFA = letterGen();
    NFA* multi_letterNFA = reptNFA(letterGen());
    // generate NFA for regex: letter+
    vec.emplace_back(letterNFA);
    vec.emplace_back(multi_letterNFA);
    NFA* letterPlusNFA = catNFA(vec);
    // generate NFA for regex: (digit|letter|_)*
    vec.clear();
    NFA* digitNFA = digitGen();
    letterNFA = letterGen();
    NFA* underscoreNFA = new NFA('_');
    vec.emplace_back(digitNFA);
    vec.emplace_back(letterNFA);
    vec.emplace_back(underscoreNFA);
    NFA* dluNFA = orNFA(vec);
    dluNFA = reptNFA(dluNFA);
    // generate NFA for regex: (letter+).(digit|letter|_)*
    vec.clear();
    vec.emplace_back(letterPlusNFA);
    vec.emplace_back(dluNFA);
    NFA* res = catNFA(vec);

    res->endNode->token = new char[10];
    strcpy(res->endNode->token, "ID");
    return res;
}

std::vector<NFA*> allReservedWordGen(){
    auto vec = std::vector<NFA*>();
    vec.emplace_back(reservedGen("main", "MAIN"));
    vec.emplace_back(reservedGen("if", "IF"));
    vec.emplace_back(reservedGen("else", "ELSE"));
    vec.emplace_back(reservedGen("break", "BREAK"));
    vec.emplace_back(reservedGen("do", "DO"));
    vec.emplace_back(reservedGen("while", "WHILE"));
    vec.emplace_back(reservedGen("void", "VOID"));
    vec.emplace_back(reservedGen("int", "INT"));
    vec.emplace_back(reservedGen("scanf", "SCANF"));
    vec.emplace_back(reservedGen("printf", "PRINTF"));
    vec.emplace_back(reservedGen("return", "RETURN"));
    return vec;
}

std::vector<NFA*> allSymbolGen(){
    auto vec = std::vector<NFA*>();
    vec.emplace_back(reservedGen("&", "AND_OP"));
    vec.emplace_back(reservedGen("|", "OR_OP"));
    vec.emplace_back(reservedGen("&&", "ANDAND"));
    vec.emplace_back(reservedGen("||", "OROR"));
    vec.emplace_back(reservedGen("<=", "LTEQ"));
    vec.emplace_back(reservedGen(">=", "GTEQ"));
    vec.emplace_back(reservedGen("==", "EQ"));
    vec.emplace_back(reservedGen(">", "GT"));
    vec.emplace_back(reservedGen("<", "LT"));
    vec.emplace_back(reservedGen("!=", "NOTEQ"));
    vec.emplace_back(reservedGen("=", "ASSIGN"));
    vec.emplace_back(reservedGen("(", "LPAR"));
    vec.emplace_back(reservedGen(")", "RPAR"));
    vec.emplace_back(reservedGen("{", "LBRACE"));
    vec.emplace_back(reservedGen("}", "RBRACE"));
//    vec.emplace_back(reservedGen(".")); what the fuck is this
    vec.emplace_back(reservedGen(",", "COMMA"));
    vec.emplace_back(reservedGen(";", "SEMI"));
    vec.emplace_back(reservedGen("+", "PLUS"));
    vec.emplace_back(reservedGen("-", "MINUS"));
    vec.emplace_back(reservedGen("*", "MUL_OP"));
    vec.emplace_back(reservedGen("/", "DIV_OP"));
    vec.emplace_back(reservedGen("[", "LSQUARE"));
    vec.emplace_back(reservedGen("]", "RSQUARE"));
    vec.emplace_back(reservedGen("!", "NOT_OP"));
    vec.emplace_back(reservedGen(">>", "SHR_OP"));
    vec.emplace_back(reservedGen("<<", "SHL_OP"));
    return vec;
}