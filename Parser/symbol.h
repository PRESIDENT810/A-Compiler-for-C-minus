#ifndef PROJECT3_SYMBOL_H_
#define PROJECT3_SYMBOL_H_

#include <cstring>

//
// the enumeration class for terminal & non-terminal symbols
//
enum class Symbol{
    // non-terminal symbols
    program = 0,
    varDecls = 1,
    varDeclsPostfix = 2,
    varDecl = 3,
    declList = 4,
    declListPostfix = 5,
    decl = 6,
    declPostfix = 7,
    codeBlock = 8,
    stmts = 9,
    stmtsPostfix = 10,
    stmt = 11,
    controlStmt = 12,
    rwStmt = 13,
    assignStmt = 14,
    assignStmtPostfix = 15,
    ifStmts = 16,
    ifStmtsPostfix = 17,
    ifStmt = 18,
    whileStmt = 19,
    dowhileStmt = 20,
    returnStmt = 21,
    rStmt = 22,
    wStmt = 23,
    exp = 24,
    realExp = 25,
    realExpPostfix = 26,
    OpExp = 27,
    // reversed words
    INT = 28,
    MAIN = 29,
    VOID = 30,
    BREAK = 31,
    DO = 32,
    ELSE = 33,
    IF = 34,
    WHILE = 35,
    RETURN = 36,
    READ = 37,
    WRITE = 38,
    // special symbols
    LBRACE  =39,
    RBRACE = 40,
    LSQUARE = 41,
    RSQUARE = 42,
    LPAR = 43,
    RPAR = 44,
    SEMI = 45,
    PLUS = 46,
    MINUS = 47,
    MUL_OP = 48,
    DIV_OP = 49,
    AND_OP = 50,
    OR_OP = 51,
    NOT_OP = 52,
    ASSIGN = 53,
    LT = 54,
    GT = 55,
    SHL_OP = 56,
    SHR_OP = 57,
    EQ = 58,
    NOTEQ = 59,
    LTEQ = 60,
    GTEQ = 61,
    ANDAND = 62,
    OROR = 63,
    COMMA = 64,
    // regex symbols
    INTNUM = 65,
    ID = 66,
    // null string
    nullStr = 67,
};

#endif //PROJECT3_SYMBOL_H